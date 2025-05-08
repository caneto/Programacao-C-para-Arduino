//-----------------------------------------------------------------------------------------
// Função   : Este programa tem como objetivo implementar a programação de horários para
//            acionamento e desligamento de relés, com suporte a operações manuais e 
//            automáticas através de uma interface HTML numa aplicação AsyncWebServer  
//            para ESP8266 com interação com o Navegador via WebSocket.
//
// Funções adicionais
//
//   1) Inclusão de Alias no DNS da Rede Local (mDNS)
//   2) Sincronização do Relógio interno com o Serviço NTP 
//   3) Atualização do Código via Interface Web (ElegantOTA)
//   4) Generalização para mais ou menos Relés (MAX_RELES) com poucos ajustes no CALLBACK
//      Veja as instruções em alguns pontos do código  
//   5) Utilização de Ticker para evitar TIMER de hardware que são mais restritos em quantidade 
//      e podem ser utilizados por outras bibliotecas, gerando conflito
//
//  Atributos do Agendamento
//
//    idEvento             =>  Identificação do Evento no cadastro
//    horaInicial          =>  Hora de Início da Ativação (hh:mm) 
//    horaFinal            =>  Hora final da Desativação  (hh:mm)
//    repetirCadaMinutos   =>  Repetição do Evento ao longo do dia (min)
//    descricao            =>  Descrição do Evento (<50)
//    releSelecionado      =>  Identificação do Rele para o Evento
//    diaSemana            =>  Dias da Semanas para o Evento [0=Dom 1=Seg 2=Ter 3=Qua 4=Qui 5=Sex 6=Sab]
//    dataBase             =>  Data Base no Formato YYYY-MM-DD
//    condicao             =>  Operador de comparação (ex.: "<", "<=", "=", "!=", ">", ">=")      
//
// Autor     : Dailton Menezes
// Versão    : 1.0 Jan/2025 
// Correção 
// Author    : Carlos Alberto 
// Versão    : 1.1 Abr/2025
// 
// Patch no src do ElegantOta
// 1) Vá para o diretório de bibliotecas do Arduino.
// 2) Abra a pasta ElegantOTA e depois abra a pasta src.
// 3) Abra o arquivo ElegantOTA.h. Pode abrir com o bloco de notas.
// 4) Com o arquivo aberto procure a linha: #define ELEGANTOTA_USE_ASYNC_WEBSERVER 0
// 5) Mude o valor no final da linha de 0 para 1
// 6) Salve e feche o arquivo e compile o programa novamente.
//-----------------------------------------------------------------------------------------

//--------------------------------------
// Inclusão das Bibliotecas Necessárias
//--------------------------------------

#include <ESP8266WiFi.h>                     // Biblioteca para a rede wifi
#include <ESP8266Ping.h>                     // Biblioteca para o Ping
#include <ESP8266mDNS.h>                     // Biblioteca para adicionar no DNS
#include <ESPAsyncTCP.h>                     // Biblioteca pré-requisito para AsyncWebServer
#include <FS.h>                              // Biblioteca para SPIFFS    
#include <ESPAsyncWebServer.h>               // Biblioteca para o AsyncWebServer ---> Versão 3.6.0
#include <ElegantOTA.h>                      // Biblioteca para atualização via Web ---> Versão 3.1.1
#include <ArduinoJson.h>                     // Biblioteca para manipulação de JSON
#include <Ticker.h>                          // Biblioteca para eventos temporais
#include <vector>                            // Biblioteca para listas na memória
#include <ctime>                             // Biblioteca para manipulação de tempo

//----------------------------
// Definições para o programa
//----------------------------

#define MAX_RELES            1               // Números de Relés
#define pinRELE1             0               // Porta para o Rele 1
#define pinRELE2             26              // Porta para o Rede 2
#define pinBoot              0               // Pino do botão para forçar a entrada no WifiManager  
#define RELE_ON              LOW             // Estado para o Relé Ativo
#define RELE_OFF             HIGH            // Estado para Relé desativado
#define DEFAULT_DNS_NAME     "esp8266"       // Nome para adicionar no mDNS
#define LED_BUILTIN          2               // Pino do Led BuiltIn interno para indicar Wifi ON/OFF
#define JSON_AGENDA_FILE     "/agenda.json"  // Arquivo JSON para os Agendamentos
#define JSON_CONFIG_FILE     "/config.json"  // Arquivo JSON de configuração
#define BAUDRATE             115200          // Baudrate para a Console
#define MAX_EDIT_LEN         30              // Tamanho máximo de campos de EDIT
#define MAX_NUM_LEN          4               // Tamanho máximo de campos NUMÉRICO
#define USER_UPDATE          "admin"         // Usuário para atualização via OTA
#define PASS_UPDATE          "esp8266@agenda"  // Senha para atualização via OTA
#define DEFAULT_PASS_AP      "12345678"      // Senha default do modo AP WifiManager
#define DEFAULT_NTP_SERVER   "a.st1.ntp.br"  // Servidor NTP do Brasil
#define DEFAULT_TZ_INFO      "<-03>3"        // TimeZone do Brasil
#define DEFAULT_REFRESH_NTP  720             // Intervalo para Refresh do Horário NTP Server (min)
#define CHECK_INTERNET       30000           // Intervalo para verificar se a Internet está ativa (ms)
#define CICLO_CLEANUP        10000           // Intervalo para verifica o cleanup de WebSocket inativo 
#define MAX_HTTP_CONNECTIONS 1               // Máximo de conexões ativas


//-----------------------------------------
// Estrutura para armazenar os agendamentos
//-----------------------------------------

struct Agendamento 
{
    String idEvento;                         // Identificação do Evento no cadastro
    String horaInicial;                      // Hora de Início da Ativação 
    String horaFinal;                        // Hora final da Desativação
    int repetirCadaMinutos;                  // Repetição do Evento ao longo do dia
    String descricao;                        // Descrição do Evento
    int releSelecionado;                     // Identificação do Rele para o Evento
    std::vector<int> diaSemana;              // Dias da Semanas para o Evento
    String dataBase;                         // Data Base no Formato YYYY-MM-DD
    String condicao;                         // Operador de comparação (ex.: "<", "<=", "=", "!=", ">", ">=")    
};

//------------------
// Variáveis globais
//------------------

AsyncWebServer server(80);                   // Servidor Web
AsyncWebSocket ws("/ws");                    // Servidor Websocket
std::vector<Agendamento> dbAgenda;           // Lista de Eventos na memória
int pinRele[MAX_RELES] = {pinRELE1};         // Vetor de Pinos dos Relés
bool statusRele[MAX_RELES] = {false};        // Vetor de Estado dos Relés
Ticker timerReles[MAX_RELES];                // Vetor de Timers dos Relés
Ticker verificaAgendamentosTimer;            // Timer para varredura do agendamento 
Ticker refreshNTPTimer;                      // Timer para resincronização do horário com o NTP Server 
Ticker rebootTimer;                          // Timer para reiniciar o ESP
volatile bool buttonState = false;           // Estado do botão Boot para Reconfiguração do WiFi
IPAddress ip (1, 1, 1, 1);                   // The remote ip to ping, DNS do Google
JsonDocument dbParm;                         // Base de dados de parâmetros
unsigned long lastInternetCheck=0;           // Última verificação se a Internet está ativa
bool sincronizouNTP=false;                   // Se fez uma sincronização com NTP Server
unsigned long lastCleanUp=0;                 // ùltima verificação de CleanUp 
int activeConnections = 0;                   // Número de conexões ativas
String modos[2] = {"AP","WiFi"};             // Modos de Operação

//---------------------------------------------
// Variáveis para controle do OTA
//---------------------------------------------

bool autoRebootOTA = true;                  // Se deve fazer autoreboot após a atualização OTA
String user_OTA = USER_UPDATE;              // Usuário para atualização OTA
String pass_OTA = PASS_UPDATE;              // Senha para atualização OTA

//---------------------------------------------
// Variáveis para controle dos Parâmetros
//---------------------------------------------

String NTP_SERVER  = DEFAULT_NTP_SERVER;    // Servidor NTP
String TZ_INFO     = DEFAULT_TZ_INFO;       // String do TimeZone 
String DNS_NAME    = DEFAULT_DNS_NAME;      // Nome Default para o DNS
String ssid_config = "";                    // SSID para o modo  AP de Configuração 
String pass_AP     = DEFAULT_PASS_AP;       // Senha para o modo AP de Configuração
String ssid_wifi   = "";                    // SSID para o modo  AP de Configuração 
String pass_wifi   = "";                    // Senha para o modo AP de Configuração
String gpios       = "";                    // GPIO's dos Relés separados por , 
int intervaloNTP   = DEFAULT_REFRESH_NTP;   // Para receber o Intervalo NTP (min)
bool reset_agendamentos = false;            // Reset Agenda,entos Bool
int modo_operacao  = 0;                     // Modo de Operação 0=AP 1=WiFi
String erro_msg = "";                       // Erro na Configuração

//--------------------------------------
// Define o JSON Default dos Parâmetros
//--------------------------------------

const char dbDefault[] PROGMEM = R"(
{
   "ssid": "",
   "senhaSSID": "",
   "senhaAP": "12345678", 
   "DnsName": "esp8266",
   "NTPServer": "a.st1.ntp.br",
   "Timezone": "<-03>3", 
   "intervaloNTP": "720",
   "usuarioOTA": "admin",
   "senhaOTA": "esp8266@agenda",
   "autorebootOTA": true,
   "resetAgendamentos": false
})";

//------------------------------
// HTML para Agendamentos
//------------------------------

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controle de WiFi ESP8266 Relay</title>
    <style>
        .container {
            border: 1px solid #000;
            padding: 5px;
            margin-bottom: 5px;
            border-radius: 10px;
        }

        .container h2 {
            font-size: 18px;
            margin-top: 5px;
        }

        header {
            width: 100%;
            background-color: black;
            color: white;
            text-align: center;
            padding: 1px 0;
            margin-bottom: 3px;
        }

        button {
            width: 90px;
            margin: 5px;
            padding: 10px;
            font-size: 16px;
            background-color: #4CAF50;
            color: #fff;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }

        .btn-outro {
            width: 90px;
            margin: 5px;
            padding: 10px;
            font-size: 16px;
            background-color: #0074E4;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }

        #repetirCadaMinutos {
            width: 90px;
        }
        #descricao {
            width: 205px;
        }
    </style>
    <script>
        let socket;
 
        function iniciarWebSocket() {
            socket = new WebSocket('ws://' + window.location.hostname + '/ws');

            socket.onopen = function() {
                console.log("Conexão WebSocket aberta.");
                fillSelect();
                atualizarStatus();
            };

            socket.onmessage = function(event) {
                tratarMensagem(event);
            };

            socket.onclose = function() {
                console.log("Conexão WebSocket fechada.");
            };

        }

       window.onload = iniciarWebSocket;        

        function enviarMensagem(data) {
            if (socket.readyState === WebSocket.OPEN) {
                socket.send(JSON.stringify(data));
            } else {
                console.error("WebSocket não está aberto.");
            }
        }

        function tratarMensagem(event) {
            const response = JSON.parse(event.data);
            if (response.event === 'getAgendaList') {
                const select = document.getElementById('listboxAgendamentos');
                select.innerHTML = '<option value="">Selecione um agendamento...</option>';
                if (Array.isArray(response.agendaList)) {
                    response.agendaList.forEach(item => {
                        const option = document.createElement('option');
                        option.value = item.idEvento;
                        option.textContent = item.descricao;
                        select.appendChild(option);

                       // Seleciona o `idEvento` após atualização
                       if (item.idEvento === document.getElementById('idEvento').value) {
                           select.value = item.idEvento;
                       }
                    });
                } else {
                    console.error("A resposta não contém um array de agendamentos válido.");
                }
            } %getstatus%
            } else if (response.event === 'getAgenda') {
                document.getElementById('idEvento').value = response.idEvento; // Atualiza idEvento com o valor do agendamento existente
                // Atualizar campos do formulário
                document.getElementById('horaInicial').value = response.horaInicial;
                document.getElementById('horaFinal').value = response.horaFinal;
                document.getElementById('repetirCadaMinutos').value = response.repetirCadaMinutos;
                document.getElementById('descricao').value = response.descricao;
                document.getElementById('releSelecionado').value = response.releSelecionado; // Ajuste para o campo do relé
                document.getElementById('dataBase').value = response.dataBase; // Atualiza o campo dataBase
                document.getElementById('condicao').value = response.condicao; // Atualiza o campo condicao                
                ['domingo', 'segunda', 'terca', 'quarta', 'quinta', 'sexta', 'sabado'].forEach((dia, index) => {
                    document.getElementById(dia).checked = response.diaSemana.includes(index);
                });
             } else if (response.event === 'updateAgenda' && response.status === 'ok') {
                  fillSelect(response.idEvento);  // Atualiza o `select` e sincroniza `idEvento`
             } else if (response.event === 'deleteAgenda' && response.status === 'ok') {
                  fillSelect();       // Atualiza a lista após uma exclusão
                  limparFormulario(); // limpa o formulário após exclusão
             } else if (response.event === 'controlRele' && response.status === 'ok') {
                  atualizarStatus(); // Atualiza a lista após uma exclusão                  
             }           
        }

        function fillSelect(selectedId = null) {
            enviarMensagem({ event: 'getAgendaList' });
            
            const idEventoField = document.getElementById('idEvento');
            
            // Sincronizar `idEvento` e limpar após exclusão
            if (selectedId) {
                idEventoField.value = selectedId;
            } else {
                idEventoField.value = ""; // Limpar `idEvento` após exclusão
            }
        }

        // Função para limpar o formulário
        function limparFormulario() {
            document.getElementById('idEvento').value = "";
            document.getElementById('horaInicial').value = "";
            document.getElementById('horaFinal').value = "";
            document.getElementById('repetirCadaMinutos').value = "";
            document.getElementById('descricao').value = "";
            document.getElementById('releSelecionado').value = "";
            document.getElementById('dataBase').value = "";
            document.getElementById('condicao').value = "";
            ['domingo', 'segunda', 'terca', 'quarta', 'quinta', 'sexta', 'sabado'].forEach(dia => {
                document.getElementById(dia).checked = false;
            });
        }        

        function recuperarAgendamento() {
            const idEvento = document.getElementById('listboxAgendamentos').value;
            if (idEvento) {
                enviarMensagem({ event: 'getAgenda', idEvento: idEvento });
            }
        }

        function enviarAgendamento() {
            const idEventoField = document.getElementById('idEvento');
            const idEvento = idEventoField.value; // Agora usamos apenas o valor atual
        
            const horaInicial = document.getElementById('horaInicial').value;
            const horaFinal = document.getElementById('horaFinal').value;
            const repetirCadaMinutos = parseInt(document.getElementById('repetirCadaMinutos').value, 10);
            const descricao = document.getElementById('descricao').value.trim();
            const releSelecionado = parseInt(document.getElementById('releSelecionado').value, 10);
            const dataBase = document.getElementById('dataBase').value;
            const condicao = document.getElementById('condicao').value;            

            console.log("enviarAgendamento idEvento:", idEventoField.value); 

            // Valida a DataBase
            
            if (!dataBase) {
                alert('Por favor, selecione uma data válida para a Data Base.');
                return;
            }

            if (!condicao) {
                alert('Por favor, selecione uma condição de comparação válida.');
                return;
            }   

            if (!horaInicial || !horaFinal || horaInicial >= horaFinal) {
                alert('Hora Inicial deve ser menor que Hora Final.');
                return;
            }

            if (isNaN(repetirCadaMinutos) || repetirCadaMinutos < 0) {
                alert('Repetir a cada minutos deve ser um valor numérico maior ou igual a zero.');
                return;
            }
            if (!descricao || descricao.length > 50) {
                alert('Descrição não pode ser nula e deve ter no máximo 50 caracteres.');
                return;
            }
        
            const diasSemana = [];
            ['domingo', 'segunda', 'terca', 'quarta', 'quinta', 'sexta', 'sabado'].forEach((dia, index) => {
                if (document.getElementById(dia).checked) diasSemana.push(index);
            });
        
            const agendamento = {
                idEvento: idEventoField.value,
                horaInicial: horaInicial,
                horaFinal: horaFinal,
                repetirCadaMinutos: repetirCadaMinutos,
                descricao: descricao,
                rele: releSelecionado,
                dataBase: dataBase,
                condicao: condicao,                
                diaSemana: diasSemana
             };

            //console.log("JSON Enviado:", JSON.stringify(agendamento));
            enviarMensagem({ event: 'updateAgenda', data: agendamento });
            
        }

        function deletarAgendamento() {
            const userConfirmed = confirm("Tem certeza que deseja deletar o Agendamento selecionado?");
            if (!userConfirmed) {
                return;  // Sai se o usuário cancelar
            }
          
            const idEvento = document.getElementById('listboxAgendamentos').value;
            console.log("deletarAgendamento idEvento:", idEvento); 
            if (idEvento) {
                enviarMensagem({ event: 'deleteAgenda', idEvento: idEvento });
            } else {
                alert('Selecione um agendamento para deletar.');
            }
        }

        function adicionarAgendamento() {
            // Limpa o campo `idEvento` para garantir que um novo registro seja criado
            document.getElementById('idEvento').value = Date.now().toString(); // Gera um novo `idEvento` único
             console.log("adicionarAgendamento idEvento:", document.getElementById('idEvento').value); 
            enviarAgendamento();
        }

        function atualizarAgendamento() {
            enviarAgendamento();
        }        

        function ligarRele() {
            const rele = document.getElementById('selecionarRele').value;
            enviarMensagem({ event: 'controlRele', rele: parseInt(rele, 10), comando: 'ligar' });
        }

        function desligarRele() {
            const rele = document.getElementById('selecionarRele').value;
            enviarMensagem({ event: 'controlRele', rele: parseInt(rele, 10), comando: 'desligar' });
        }

        function atualizarStatus() {
            enviarMensagem({ event: 'getStatus' });
        }

        // Evento para fechar o WebSocket ao descarregar a página
        window.onbeforeunload = function() {
            if (socket) {
                socket.close();
            }
        };

        // Evento quando o VOLTAR é usado pois o cache é utilizado
        // window.addEventListener("pageshow", function(event) {
        //     if (event.persisted) {
        //         location.reload();
        //     }
        // });

        setInterval(function() {
            if (!socket || socket.readyState !== WebSocket.OPEN) {
                console.log("WebSocket inativo! Tentando reconectar...");
                iniciarWebSocket();
            }
        }, 5000);        

    </script>
</head>
<body>

    <header>
        <h3>ESP8266 Relay - Modo %modo%</h3>
    </header>

    <div class="container">
        <h2>Agendamentos</h2>
        <select id="listboxAgendamentos" onchange="recuperarAgendamento()">
            <option value="">Selecione um agendamento...</option>
        </select>
        <br><br>
        <form id="formAgendamento">
            <input type="hidden" id="idEvento" name="idEvento">

            <label for="dataBase">Data Base:</label>
            <input type="date" id="dataBase" name="dataBase" required title="Data Atual ou Futura"><br><br>
            <label for="condicao">Condição:</label>
            <select id="condicao" name="condicao" required title="Critério de comparação entre Data Processamento com DataBase">
                <option value="<"><</option>
                <option value="<="><=</option>
                <option value="=">=</option>
                <option value="!=">!=</option>
                <option value=">">></option>
                <option value=">=">>=</option>
            </select><br><br>

            <label for="horaInicial">Hora Inicial:</label>
            <input type="time" id="horaInicial" name="horaInicial" required title="Ex: 08:00"><br>
            <label for="horaFinal">Hora Final:&nbsp;&nbsp;</label>
            <input type="time" id="horaFinal" name="horaFinal" required title="Ex: 18:00"><br><br>

            <label for="releSelecionado">Relé:</label>
            <select id="releSelecionado" name="releSelecionado" title="Selecione o Relé a ser considerado no agendamento">
                %relelist1%
            </select><br><br>

            <label>Dias da Semana:</label>
            <input type="checkbox" id="domingo" name="diaSemana" value="0"> Domingo
            <input type="checkbox" id="segunda" name="diaSemana" value="1"> Segunda
            <input type="checkbox" id="terca" name="diaSemana" value="2"> Terça
            <input type="checkbox" id="quarta" name="diaSemana" value="3"> Quarta
            <input type="checkbox" id="quinta" name="diaSemana" value="4"> Quinta
            <input type="checkbox" id="sexta" name="diaSemana" value="5"> Sexta
            <input type="checkbox" id="sabado" name="diaSemana" value="6"> Sábado<br><br>

            <label for="repetirCadaMinutos">Repetir a cada (minutos):</label>
            <input type="number" id="repetirCadaMinutos" name="repetirCadaMinutos" required placeholder="Ex: 60"><br><br>

            <label for="descricao">Descrição:</label>
            <input type="text" id="descricao" name="descricao" required placeholder="Ex: Ligar bomba d'água"><br><br>

            <button type="button" onclick="adicionarAgendamento()">Adicionar</button>
            <button type="button" onclick="atualizarAgendamento()">Atualizar</button>
            <button type="button" onclick="deletarAgendamento()" class="btn-outro">Deletar</button>
        </form>
    </div>

    <div class="container">
        <h2>Controle Manual</h2>
        <select id="selecionarRele" title="Selecione o Relé a ser considerado manualmente">
            %relelist2%
        </select>
        <button type="button" onclick="ligarRele()">Ligar</button>
        <button type="button" onclick="desligarRele()" class="btn-outro">Desligar</button>
    </div>

    <div class="container">
        <h2>Status dos Relés</h2>
        %statuslist%
    </div>
    <div class="container">
        <h2>Configuração</h2>
        <button type="button" onclick="location.href='/config'" class="btn-outro">Config</button>
    </div> 
</body>
</html>
)rawliteral";

//--------------------------
// HTML para a configuração
//--------------------------

const char config_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuração WiFi ESP8266 Relay</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            background-color: #f4f4f9;
        }
        header {
            width: 100%;
            background-color: black;
            color: white;
            text-align: center;
            padding: 1px 0;
            margin-bottom: 3px;
        }        
        .container {
            width: 90%;
            max-width: 500px;
            padding: 20px;
            background-color: white;
            border: 1px solid #ddd;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }
        label {
            display: block;
            margin-top: 10px;
            font-size: 1rem;
        }
        input, select {
            width: 100%;
            padding: 8px;
            margin-top: 5px;
            font-size: 1rem;
            border: 1px solid #ddd;
            border-radius: 5px;
        }
        .buttons {
            display: flex;
            justify-content: space-between;
            margin-top: 20px;
        }

        button {
            width: 90px;
            margin: 5px;
            padding: 10px;
            font-size: 16px;
            background-color: #0074E4;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }        
     
        button[type="submit"] {
            background-color: #007BFF; /* Azul */
            color: white; /* Fonte branca */
            padding: 10px 20px; /* Ajuste de tamanho */
            font-size: 16px; /* Tamanho da fonte */
            border: none; /* Sem borda */
            border-radius: 5px; /* Borda arredondada */
            cursor: pointer; /* Cursor de clique */
        }

        /* Efeito ao passar o mouse */
        button[type="submit"]:hover {
            background-color: #0056b3; /* Azul mais escuro */
        }

        }
    </style>
</head>
<body>
    <header>
        <h2>Configuração</h2>
    </header>
    <div class="container">
        
        <form method="post" action="/setConfig">
            <label for="ssid">SSID:</label>
            <input type="text" id="ssid" name="ssid" value="%ssid%" required>

            <label for="password">Senha:</label>
            <input type="password" id="passwifi" name="passwifi" value="%passwifi%" required>

            <label for="user">Usuário:</label>
            <input type="text" id="user" name="user" value="%user%">

            <label for="pass">Senha:</label>
            <input type="password" id="passuser" name="passuser" value="%passuser%">

            <label for="apid">APID:</label>
            <input type="text" id="apid" name="apid" value="%apid%" required>

            <label for="passAP">Senha AP:</label>
            <input type="password" id="passAP" name="passAP" value="%passAP%">

            <label for="dnsname">DnsName:</label>
            <input type="text" id="dnsname" name="dnsname" value="%dnsname%">

            <label for="ntpserver">NTP Server:</label>
            <input type="text" id="ntpserver" name="ntpserver" value="%ntpserver%">         

            <label for="timezone">TimeZone:</label>
            <input type="text" id="timezone" name="timezone" value="%timezone%">                

            <label for="intervaloNTP">Intervalo NTP (min):</label>
            <input type="number" id="intervaloNTP" name="intervaloNTP" value="%intervaloNTP%">

            <label for="modo_operacao">Modo de Operação:</label>
            <select id="modo_operacao" name="modo_operacao">
                <option value="0" %modo_operacao_true%>AP</option>
                <option value="1" %modo_operacao_false%>WiFi</option>
            </select>                    

            <label for="reset_agenda">Resetar Agenda:</label>
            <select id="reset_agenda" name="reset_agenda">
                <option value="true" %reset_agenda_true%>Sim</option>
                <option value="false" %reset_agenda_false%>Não</option>
            </select>                    

            <label for="autoRebootOTA">Reiniciar após OTA:</label>
            <select id="autoRebootOTA" name="autoRebootOTA">
                <option value="true" %autoRebootOTA_true%>Sim</option>
                <option value="false" %autoRebootOTA_false%>Não</option>
            </select>
            <br><br>
            <button type="button" onclick="salvarConfig()">Salvar</button>  
            <button type="button" onclick="sincronizarHora()">SetTime</button>  
            <button type="button" onclick="location.href='/update'">Update</button>
            <center><p id="statusMsg"></p></center>
        </form>
    </div>

    <script>
        let socket;

        function iniciarWebSocket() {
            socket = new WebSocket('ws://' + window.location.hostname + '/ws');

            socket.onopen = function() {
                console.log("✅ WebSocket conectado.");
            };

            socket.onmessage = function(event) {
                const response = JSON.parse(event.data);
                if (response.event === "syncTime" || response.event === "setConfig") {
                    document.getElementById("statusMsg").innerText = response.message;
                }
            };

            socket.onclose = function() {
                console.log("WebSocket desconectado.");
            };
        }

        function sincronizarHora() {
            let timestamp = Math.floor(Date.now() / 1000);
            socket.send(JSON.stringify({ event: "syncTime", timestamp: timestamp }));
        }

        function salvarConfig() {
            let configData = {
                event: "setConfig",
                data: {
                    ssid: document.getElementById("ssid").value,
                    passwifi: document.getElementById("passwifi").value,
                    user: document.getElementById("user").value,
                    passuser: document.getElementById("passuser").value,
                    apid: document.getElementById("apid").value,
                    passAP: document.getElementById("passAP").value,
                    dnsname: document.getElementById("dnsname").value,
                    ntpserver: document.getElementById("ntpserver").value,
                    timezone: document.getElementById("timezone").value,
                    intervaloNTP: parseInt(document.getElementById("intervaloNTP").value, 10),
                    modoOperacao: parseInt(document.getElementById("modo_operacao").value, 10), // Correção aqui
                    resetAgendamentos: document.getElementById("reset_agenda").value === "true",
                    autoRebootOTA: document.getElementById("autoRebootOTA").value === "true"
                }
            };
            socket.send(JSON.stringify(configData));

            // Mostra um alerta de sucesso
            alert("Configuração enviada! O ESP irá reiniciar...");

            // **Aguarda 4 segundos e redireciona para a página principal ("/")**
            setTimeout(() => {
                window.location.href = "/";
            }, 4000);  // Tempo suficiente para o ESP reiniciar            
        }

        // Evento para a carga da página
        window.onload = iniciarWebSocket;

        // Evento para fechar o WebSocket ao descarregar a página
        window.onbeforeunload = function() {
            if (socket) {
                socket.close();
            }
        };

        // Evento quando o VOLTAR é usado pois o cache é utilizado
        // window.addEventListener("pageshow", function(event) {
        //     if (event.persisted) {
        //         location.reload();
        //     }
        // });        

        // Evento para varrer a perda do socket para recriar
        setInterval(function() {
            if (!socket || socket.readyState !== WebSocket.OPEN) {
                console.log("WebSocket inativo! Tentando reconectar...");
                iniciarWebSocket();
            }
        }, 5000);        

    </script>

</body>
</html>
)rawliteral";

const char *limite PROGMEM = R"rawliteral(
<html>
<head>
    <meta charset="UTF-8">
    <title>Erro de Conexão</title>
</head>
<body>
    <h2>Máximo de conexões atingido</h2>
    <p>Tente novamente mais tarde.</p>
</body>
</html>
)rawliteral";

//--------------------------------
// Prototipação das funções usadas
//--------------------------------

// Wifi

void onGotIP(const WiFiEventStationModeGotIP& event);
void onDisconnected(const WiFiEventStationModeDisconnected& event);

// WebSocket

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len); // Trata eventos WebSocket

// Parâmetros

String saveConfigFile();                             // Persiste CPUID e Intervalo no SPIFFS do ESP32
bool loadConfigFile();                               // Recupera CPUID e Intervalo do SPIFFS do ESP32

// Auxiliares

bool getNTPtime(int sec);                            // Faz o sincronismo do relógio com  Servidor NTP
String getTimeStamp();                               // Retorna o TimeStamp (Data/Hora)            
String expandeHtml(String html);                     // Expande o HTML para o Navegador
String repeatChar(char c, int num);                  // Devolve um string com a repetição de um determinado carater
void deleteFile(const char* path);                   // Apaga o arquivo de Agendamentos no SPIFFS (uso esporádico)
int horaParaMinutos(String hora);                    // Converte hh:mm para total em minutos
bool setDNSNAME(String nome);                        // Define o DNSNAME e HOSTNAME da Rede 
void refreshNTPServer();                             // Faz  o sincronismo com Servidro NTP
bool isAuthenticated(AsyncWebServerRequest *request);// Autenticação para acessar a URL
String getDefaultID();                               // Retorna a ID default para o modo AP

// Da Aplicação

void loadAgenda();                                   // Carrega os Agendamentos do SPIFFS para a RAM
void saveAgenda();                                   // Salva a lista de agendamentos para o SPIFFS
String getStatusJSON();                              // Devolve o status dos relés em JSON
void enviarStatusParaTodosClientes();                // Envia o status dos Relés para os clientes
void alterarEstadoRele(int releIndex, bool estado);  // Altera o status de um determinado relé e avisa interface
String getReleList(int numEspacos);                  // Devolve a lista de Relés para substituição no HTML (<select>)
String getStatusList(int numEspacos);                // Devolve a lista de campos de status para substituição no HTML (<div>)
String getStatusUpdateScript(int numEspacos);        // Devolve o código para o HTML para substituição no Evento getStatus
void desligarRele(int releIndex);                    // Desliga um relé dado o seu índice
void verificarAgendamentos();                        // Faz a varredura no cadastro de agendamento para a programação
bool verificarCondicaoData(const String& condicao, const String& dataAtualStr, const String& dataBaseStr); // Faz a comparaação entre datas
void rebootESP();                                    // Faz o reboot do ESP
void initAgendamentos();                             // Inicializa os Agendamentos 
void notifyClients(String event, String status, String message); // Envia retorno de Evento

//--------------------------------desligado-----------
// Prototipação das rotinas de CALLBACK
// Ajuste caso MAX_RELES seja diferente de 2
//-------------------------------------------

void desligarRele0();                                // Callback de desligamento para o relé 1 
//void desligarRele1();                                // Callback de desligamento para o Relé 2

//------------------------
// Configurações iniciais
//------------------------

void setup() 
{
    // Inicializa a Serial
    
    Serial.begin(BAUDRATE);
    while (!Serial);

    // Define o handle para tratar os eventos do Wifi

    WiFi.onStationModeGotIP(onGotIP);
    WiFi.onStationModeDisconnected(onDisconnected);
 
    // Define o Led conectado no WiFi ou desligado quando fora 

    pinMode(LED_BUILTIN,OUTPUT);  

    // Define os Relés e inicializa

    for (int ind=0; ind < MAX_RELES; ind++)
    {
       pinMode(pinRele[ind], OUTPUT);
       digitalWrite(pinRele[ind], RELE_OFF);
       statusRele[ind] = false;
    }    

    // Tenta carregar as configurações do SPIFS

    bool ok = loadConfigFile();

    // Define o HostName para o servidor web para facilitar o acesso na rede local
    // sem conhecer o IP previamente

    Serial.print("Adicionando " + String(DNS_NAME) + " no MDNS... ");
    if (setDNSNAME(DNS_NAME))
    {
        Serial.println("adicionado corretamente no MDNS!");
    }
    else 
    {
        Serial.println("Erro ao adicionar no MDNS!");
    }    

    // Inicializa o FileSystem

    SPIFFS.begin();

    // Verifica se o usuário definou o parâmetro de reset dos agendamentos

    if (reset_agendamentos)
    {
      Serial.println("Resetando o arquivo de Agendamentos...");
      deleteFile(JSON_AGENDA_FILE);  
    }
  
    // Carrega a agenda

    loadAgenda();

    // Inicializa os Eventos para Wifi e WebSocket

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    // Credenciais para atualizações via OTA
  
    ElegantOTA.setAuth(user_OTA.c_str(),pass_OTA.c_str());

    // Habilita/Desabilita AutoRebbot após a atualização
  
    ElegantOTA.setAutoReboot(autoRebootOTA);   

    // Inicia o OTA para atualização via Web

    ElegantOTA.begin(&server);           

    if (ok && modo_operacao==1)
    {
       WiFi.begin(ssid_wifi.c_str(), pass_wifi.c_str());

       // Timeout para conexão
       unsigned long startTime = millis();
       while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
         delay(500);
         Serial.print(".");
       }

       if (WiFi.status() == WL_CONNECTED) {
          // Se chegamos até aqui é porque estamos conectados
      
          Serial.printf("\nWiFi conectado em %s ...\n",WiFi.SSID().c_str());
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());
      
          // Imprime o MAC
      
          Serial.print("MAC: ");
          Serial.println(WiFi.macAddress());
      
          // Imprime o Sinal Wifi
      
          Serial.print("Sinal: ");
          Serial.print(WiFi.RSSI());
          Serial.println(" db");  

          // Tenta sincronizar o relógio interno com o servidor NPT definido no WifiManager
          
          refreshNTPServer();
      
          // Inicializa a rota para o HTML

          server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
              if (activeConnections >= MAX_HTTP_CONNECTIONS) {
                  request->send(503, "text/html", limite);
                  return;
              }            
              if (!isAuthenticated(request)) return;              
              request->send(200, "text/html", expandeHtml(index_html));
          });

          // Página de configuração

          server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {         
              if (!isAuthenticated(request)) return;
              request->send(200, "text/html", expandeHtml(config_html));
          });

          // Inicializa o Servidor Web

          server.begin();

          // Inicializa Agendamentos

          initAgendamentos();

          // Programa a resincronização com o NTP Server de acordo com o intervalo definido no WifiManager

          refreshNTPTimer.attach(intervaloNTP*60, refreshNTPServer);

          return;       

       }
   
    }

    // Monta o SSID do modo AP para permitir a configuração
  
    ssid_config = getDefaultID();
   
    // Modo AP para configuração

    WiFi.softAP(ssid_config.c_str(),pass_AP.c_str());
    Serial.println("\nEntrando no modo AP...");
    Serial.println(WiFi.softAPIP());

    // Página Principal - Relés

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
        if (activeConnections >= MAX_HTTP_CONNECTIONS) {
            request->send(503, "text/html", limite);
            return;
        }        
        if (!isAuthenticated(request)) return;         
        request->send(200, "text/html", expandeHtml(index_html));
    });

    // Página Secundária - Configuração

    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {   
        if (!isAuthenticated(request)) return;
        request->send(200, "text/html", expandeHtml(config_html));
    });

    server.begin();

    // Inicializa Agendamentos

    initAgendamentos();    

}

//----------------------------
// Loop principal do Programa
//----------------------------

void loop() 
{
   //----------------------------------------------------------
   // Função 1 : Verifica se Internet ativa 
   //-----------------------------------------------------------

    if (modo_operacao==1 && (millis()-lastInternetCheck) > CHECK_INTERNET)
    {
       lastInternetCheck = millis();  

       if (!Ping.ping(ip,4)) 
       {
          // Desliga o LED_BUILTIN

          digitalWrite(LED_BUILTIN,HIGH);

          Serial.println("Sem Internet no momento...\n");     
                     
       }
       else 
       {
          // Ligar o LED_BUILTIN

          digitalWrite(LED_BUILTIN,LOW);

          // Verifica se ainda não sincronizou NTP Server

          if (!sincronizouNTP)
          {
             // Tenta sincronizar o relógio interno com o servidor NTP definido no WifiManager
          
             refreshNTPServer();                   
          }
       }

    }

   //----------------------------------------------------------
   // Função 2 : Limpa as conexões websocket perdidas por algum 
   //            motivo (ex: Navegador fechado)
   //-----------------------------------------------------------
     
    if (millis() - lastCleanUp > CICLO_CLEANUP)
    {
       lastCleanUp = millis();
       ws.cleanupClients();
       activeConnections = ws.count(); // Atualiza com a contagem real
       Serial.printf("N. Ativos: %d\n", activeConnections);
    }

   //--------------------------------------------------------------
   // Função 3 : checa o OTA para saber se há atualização
   //--------------------------------------------------------------  

   ElegantOTA.loop();       

   //--------------------------------------------------------------
   // Função 4 : refresh no mDNS
   //--------------------------------------------------------------  

   MDNS.update();  // Mantém o mDNS ativo
}

//-----------------------------------
// Carrega os agendamentos do SPIFFS
//-----------------------------------

void loadAgenda() 
{
    if (SPIFFS.begin()) 
    {
        if (SPIFFS.exists(JSON_AGENDA_FILE)) 
        {
            File file = SPIFFS.open(JSON_AGENDA_FILE, "r");
            if (file) 
            {
                JsonDocument doc;
                DeserializationError error = deserializeJson(doc, file);
                if (!error) 
                {
                    Serial.println("Agendamentos recuperados do SPIFFS...");
                    //serializeJsonPretty(doc, Serial);
                    Serial.println(); 
                  
                    for (JsonObject obj : doc.as<JsonArray>()) 
                    {
                        Agendamento ag;
                        ag.idEvento = obj["idEvento"].as<String>();
                        ag.horaInicial = obj["horaInicial"].as<String>();
                        ag.horaFinal = obj["horaFinal"].as<String>();
                        ag.repetirCadaMinutos = obj["repetirCadaMinutos"].as<int>();
                        ag.descricao = obj["descricao"].as<String>();
                        ag.releSelecionado = obj["releSelecionado"].as<int>();
                        ag.dataBase = obj["dataBase"].as<String>();
                        ag.condicao = obj["condicao"].as<String>();
                        for (int dia : obj["diaSemana"].as<JsonArray>()) 
                        {
                            ag.diaSemana.push_back(dia);
                        }
                        dbAgenda.push_back(ag);
                    }
                }
                file.close();
            }
        }
    }
}

//---------------------------------
// Salva os agendamentos no SPIFFS
//---------------------------------

void saveAgenda() 
{
    JsonDocument doc;
    for (const auto& ag : dbAgenda) 
    {
        JsonObject obj = doc.createNestedObject();
        obj["idEvento"] = ag.idEvento;
        obj["horaInicial"] = ag.horaInicial;
        obj["horaFinal"] = ag.horaFinal;
        obj["repetirCadaMinutos"] = ag.repetirCadaMinutos;
        obj["descricao"] = ag.descricao;
        obj["releSelecionado"] = ag.releSelecionado;
        obj["dataBase"] = ag.dataBase;
        obj["condicao"] = ag.condicao;
        JsonArray dias = obj.createNestedArray("diaSemana");
        for (int dia : ag.diaSemana) 
        {
            dias.add(dia);
        }
    }

    File file = SPIFFS.open(JSON_AGENDA_FILE, "w");
    if (file) 
    {
        Serial.println("Persistindo os Agendamentos no SPIFFS...");
        serializeJson(doc, file);
        serializeJsonPretty(doc, Serial);
        Serial.println();         
        file.close();
    }
}

//------------------------------------------------
// Evento chamado no processo de conexão do Wifi
//------------------------------------------------

void onGotIP(const WiFiEventStationModeGotIP& event) {
    Serial.println("Conectado ao Wi-Fi!");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN,LOW);   // Liga o LED 
}

void onDisconnected(const WiFiEventStationModeDisconnected& event) {
    Serial.println("Wi-Fi desconectado!");
    digitalWrite(LED_BUILTIN,HIGH); // Desliga o LED 
}

//----------------------------------------------
// Função para tratar as mensagens do WebSocket
//----------------------------------------------

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    uint32_t clientId = client->id();

    if (type == WS_EVT_CONNECT) {
        activeConnections++;
        Serial.printf("Cliente conectado: %u N. Ativos: %d\n", clientId, activeConnections);
    } 
    else if (type == WS_EVT_DISCONNECT) {
        if (--activeConnections < 0) activeConnections=0;
        Serial.printf("Cliente desconectado: %u N. Ativos: %d\n", clientId, activeConnections);
    }   
    else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len) {
            data[len] = 0;
            StaticJsonDocument<1024> doc; // Aumentado para 1024 bytes para maior capacidade
            DeserializationError error = deserializeJson(doc, data);
            if (!error) {
                String event = doc["event"].as<String>();
                Serial.printf("Evento %s recebido...\n", event.c_str());

                if (event == "getStatus") {
                    String statusJSON = getStatusJSON();
                    client->text(statusJSON);
                } 
                else if (event == "syncTime") { // Evento para sincronizar o horário
                    Serial.println("Sincronizando horário com o navegador...");
                    
                    // Recebe o timestamp do navegador (segundos desde 1970)
                    unsigned long epochTime = doc["timestamp"].as<unsigned long>();

                    // Atualiza o relógio interno do ESP
                    struct timeval tv;
                    tv.tv_sec = epochTime;
                    tv.tv_usec = 0;
                    settimeofday(&tv, NULL);

                    //Serial.printf("Novo horário definido: %lu\n", epochTime);

                    // Responde ao navegador confirmando a sincronização

                    notifyClients("syncTime", "ok", "Horário sincronizado!");

                }    
                else if (event == "setConfig") { // Evento para salvar configurações
                    //Serial.println("Processando setConfig...");
                    //serializeJsonPretty(doc, Serial);
                    
                    ssid_wifi          = doc["data"]["ssid"].as<String>();
                    pass_wifi          = doc["data"]["passwifi"].as<String>();
                    user_OTA           = doc["data"]["user"].as<String>();
                    pass_OTA           = doc["data"]["passuser"].as<String>();
                    ssid_config        = doc["data"]["apid"].as<String>();
                    pass_AP            = doc["data"]["passAP"].as<String>();
                    DNS_NAME           = doc["data"]["dnsname"].as<String>();
                    NTP_SERVER         = doc["data"]["ntpserver"].as<String>();
                    TZ_INFO            = doc["data"]["timezone"].as<String>();
                    intervaloNTP       = doc["data"]["intervaloNTP"].as<int>();
                    modo_operacao      = doc["data"]["modoOperacao"].as<int>();
                    reset_agendamentos = doc["data"]["resetAgendamentos"].as<bool>();                    
//                    modo_operacao      = doc["data"]["modo_operacao"].as<int>();
//                    reset_agendamentos = doc["data"]["reset_agenda"].as<bool>();
                    autoRebootOTA      = doc["data"]["autoRebootOTA"].as<bool>();

                    String msg = saveConfigFile();

                    if (msg.length()==0)
                    {
                        msg = "Configuração salva com sucesso!";
                        Serial.println(msg);

                        // Responde ao navegador confirmando a sincronização

                        notifyClients("setConfig", "ok", msg);                        

                        // **Agenda o Reboot para 2 segundos depois**

                        rebootTimer.once(3, rebootESP);
                    }
                    else notifyClients("setConfig", "erro", msg);  

                }                            
                else if (event == "getAgendaList") {
                    StaticJsonDocument<1024> response;
                    response["event"] = "getAgendaList";
                    JsonArray array = response.createNestedArray("agendaList");
                    for (const auto& ag : dbAgenda) {
                        JsonObject obj = array.createNestedObject();
                        obj["idEvento"] = ag.idEvento;
                        obj["descricao"] = ag.descricao;
                    }
                    String output;
                    serializeJson(response, output);
                    client->text(output);
                } 
                else if (event == "getAgenda") {
                    String idEvento = doc["idEvento"].as<String>();
                    for (const auto& ag : dbAgenda) {
                        if (ag.idEvento == idEvento) {
                            StaticJsonDocument<512> response;
                            response["event"] = "getAgenda";
                            response["idEvento"] = ag.idEvento;
                            response["horaInicial"] = ag.horaInicial;
                            response["horaFinal"] = ag.horaFinal;
                            response["repetirCadaMinutos"] = ag.repetirCadaMinutos;
                            response["descricao"] = ag.descricao;
                            response["dataBase"] = ag.dataBase;
                            response["condicao"] = ag.condicao;
                            response["releSelecionado"] = ag.releSelecionado; // Incluído para enviar o relé selecionado
                            JsonArray dias = response.createNestedArray("diaSemana");
                            for (int dia : ag.diaSemana) {
                                dias.add(dia);
                            }
                            String output;
                            serializeJson(response, output);
                            client->text(output);
                            break;
                        }
                    }
                } 
                else if (event == "updateAgenda") {
                    Agendamento ag;
                    ag.idEvento = doc["data"]["idEvento"].as<String>();
                    ag.horaInicial = doc["data"]["horaInicial"].as<String>();
                    ag.horaFinal = doc["data"]["horaFinal"].as<String>();
                    ag.repetirCadaMinutos = doc["data"]["repetirCadaMinutos"].as<int>();
                    ag.descricao = doc["data"]["descricao"].as<String>();
                    ag.releSelecionado = doc["data"]["rele"].as<int>(); // Adicionado para receber o relé selecionado
                    ag.dataBase = doc["data"]["dataBase"].as<String>();
                    ag.condicao = doc["data"]["condicao"].as<String>();                    
                    ag.diaSemana.clear();
                    for (int dia : doc["data"]["diaSemana"].as<JsonArray>()) {
                        ag.diaSemana.push_back(dia);
                    }

                    bool updated = false;
                    for (auto& existingAg : dbAgenda) {
                        if (existingAg.idEvento == ag.idEvento) {
                            existingAg = ag;
                            updated = true;
                            break;
                        }
                    }

                    //Serial.printf("Evento: updateAgenda idEvento: %s updated: %d\n",ag.idEvento.c_str(),updated);

                    if (!updated) {
                        dbAgenda.push_back(ag);
                    }

                    // Persiste a Agenda 
                    
                    saveAgenda();

                    // Inclui `idEvento` na resposta JSON para manter a sincronização
                    
                    String response;
                    StaticJsonDocument<128> doc;
                    doc["event"] = "updateAgenda";
                    doc["status"] = "ok";
                    doc["idEvento"] = ag.idEvento;
                    serializeJson(doc, response);
                    client->text(response);
                    
                } 
                else if (event == "deleteAgenda") {
                    String idEvento = doc["idEvento"].as<String>();
                    //Serial.printf("Evento: deleteAgenda idEvento: %s \n",idEvento.c_str());
                    dbAgenda.erase(
                        std::remove_if(dbAgenda.begin(), dbAgenda.end(), [&](Agendamento& ag) {
                            return ag.idEvento == idEvento;
                        }),
                        dbAgenda.end()
                    );
                    saveAgenda();
                    client->text("{\"event\":\"deleteAgenda\",\"status\":\"ok\"}");
                } 
                else if (event == "controlRele") {
                    int rele = doc["rele"].as<int>();
                    String comando = doc["comando"].as<String>();

                    if (rele >= 0 && rele < sizeof(pinRele) / sizeof(pinRele[0])) { 
                        // Verifica se o índice está dentro dos limites
                        statusRele[rele] = comando == "ligar" ? true : false;
                        digitalWrite(pinRele[rele], comando == "ligar" ? RELE_ON : RELE_OFF);
                        client->text("{\"event\":\"controlRele\",\"status\":\"ok\"}");
                    } else {
                        client->text("{\"event\":\"controlRele\",\"status\":\"error\",\"message\":\"Índice de relé inválido\"}");
                    }
                }
            }
        }
    }
}

//----------------------------------------------------
// Função para expandir o HTML à minha maneira pois o 
// pré-processador do C++ usa % como delimitor e no
// HTML há outras ocorrências de % que gerariam erro 
//----------------------------------------------------

String expandeHtml(String html)
{
  html.replace("%iplocal%",WiFi.localIP().toString());
  html.replace("%relelist1%",getReleList(16).c_str());
  html.replace("%relelist2%",getReleList(12).c_str());
  html.replace("%statuslist%",getStatusList(8).c_str());
  html.replace("%getstatus%",getStatusUpdateScript(14).c_str());
  html.replace("%ssid%",ssid_wifi.c_str());
  html.replace("%passwifi%",pass_wifi.c_str());
  html.replace("%user%",user_OTA.c_str());
  html.replace("%passuser%",pass_OTA.c_str());
  html.replace("%apid%",getDefaultID().c_str());
  html.replace("%passAP%",pass_AP.c_str());
  html.replace("%dnsname%",DNS_NAME.c_str());
  html.replace("%ntpserver%",NTP_SERVER.c_str());
  html.replace("%timezone%",TZ_INFO.c_str());
  html.replace("%intervaloNTP%",String(intervaloNTP).c_str());
  html.replace("%modo_operacao_true%", modo_operacao == 0 ? "selected" : "");
  html.replace("%modo_operacao_false%", modo_operacao == 1 ? "selected" : "");   
  html.replace("%reset_agenda_true%", reset_agendamentos ? "selected" : "");
  html.replace("%reset_agenda_false%", reset_agendamentos ? "" : "selected");  
  html.replace("%autoRebootOTA_true%", autoRebootOTA ? "selected" : "");
  html.replace("%autoRebootOTA_false%", autoRebootOTA ? "" : "selected");
  html.replace("%modo%",modos[modo_operacao]);  
  //html.replace("%config%",modo_operacao == 1 ? config_macro : "");
  //html.replace("%reles%",modo_operacao == 0 ? reles_macro : "");
  //html.replace("%rota%", modo_operacao == 0 ? "/reles" : "/");   
  return html;
}

//--------------------------------------
// Função para obter o status dos relés
//--------------------------------------

String getStatusJSON() 
{
    StaticJsonDocument<256> doc;
    doc["event"] = "getStatus";
    String nome;
    for (int ind=0;ind<MAX_RELES;ind++)
    {
       nome = "rele" + String(ind+1);
       doc[nome] = statusRele[ind];
    }
    String output;
    serializeJson(doc, output);
    return output;
}

//----------------------------------------------------------
// Função para avisar aos clientes que Status de Relé mudou
//----------------------------------------------------------

void enviarStatusParaTodosClientes() 
{
    String statusJSON = getStatusJSON(); // Função que cria o JSON de status
    ws.textAll(statusJSON); // Envia para todos os clientes conectados
}

//------------------------------------------------------------
// Função para alterar o estado do Rele e avisar na interface
//------------------------------------------------------------

void alterarEstadoRele(int releIndex, bool estado) 
{
    digitalWrite(pinRele[releIndex], estado ? RELE_ON : RELE_OFF);
    statusRele[releIndex] = estado;
    enviarStatusParaTodosClientes(); 
}

//---------------------------------------------------
// Função para retornar a lista de Relés para o HTML
//---------------------------------------------------

String getReleList(int numEspacos)
{
   String lista = "";
   String espacos = repeatChar(' ',numEspacos);

   for (int ind=0;ind<MAX_RELES;ind++)
   {
      lista += "<option value=\"" + String(ind) + "\">Relé " + String(ind+1) + "</option>\n" + espacos;
   }
   return lista; 
}

//---------------------------------------------------
// Função para retornar a lista de Status dos Relés 
//---------------------------------------------------

String getStatusList(int numEspacos) 
{
    String lista = ""; 
    String espacos = repeatChar(' ',numEspacos);
    
    for (int ind = 0; ind < MAX_RELES; ind++) {
        lista += "<div>Relé " + String(ind + 1) + ": <span id=\"statusRele" + String(ind + 1) + "\">Desligado</span></div>\n" + espacos;
    }
    return lista;
}

//--------------------------------------------------------------------------
// Função para gerar o código JavaScript de atualização de status dos relés
//--------------------------------------------------------------------------

String getStatusUpdateScript(int numEspacos) 
{
    String script = "else if (response.event === 'getStatus') {\n";
    String espacos = repeatChar(' ', numEspacos);

    for (int ind = 0; ind < MAX_RELES; ind++) {
        script += espacos + "document.getElementById('statusRele" + String(ind + 1) + "').textContent = response.rele" + String(ind + 1) + " ? '🟢 Ligado' : '🔴 Desligado';\n";
    }
    return script;
}

//---------------------------------------------------
// Função para devolver uma repetição de um caracter
//---------------------------------------------------

String repeatChar(char c, int num)
{
    // Gera a string de `numEspacos` espaços
    String result = "";
    for (int i = 0; i < num; i++) 
    {
        result += c;
    }   
    return result;  
}

//----------------------------------------------
// Função para delatar arquivos do SPIFFS
// Útil para resetar estados iniciais do jogo
//----------------------------------------------

void deleteFile(const char* path) 
{
    // Verifica se o arquivo existe
    if (SPIFFS.exists(path)) 
    { 
        if (SPIFFS.remove(path)) 
        {
            Serial.printf("Arquivo %s deletado com sucesso\n", path);
        } 
        else 
        {
            Serial.printf("Falha ao deletar o arquivo %s\n", path);
        }
    } 
    else 
    {
        Serial.printf("Arquivo %s não encontrado\n", path);
    }
}

//------------------------------------------------
// Função auxiliar para converter hora no formato 
// "HH:MM" para minutos desde meia-noite
//------------------------------------------------
   
int horaParaMinutos(String hora) 
{
    int horas = hora.substring(0, 2).toInt();
    int minutos = hora.substring(3, 5).toInt();
    return horas * 60 + minutos;
}

//---------------------------------------
// Callback genérico para desligar relés
//---------------------------------------

void desligarRele(int releIndex) 
{
    if (releIndex >= 0 && releIndex < MAX_RELES) 
    {
        digitalWrite(pinRele[releIndex], RELE_OFF);
        //statusRele[releIndex] = false;
        Serial.printf("Relé %d desligado.\n", releIndex + 1);
        //enviarStatusParaTodosClientes();
        alterarEstadoRele(releIndex,false);
    }
}

//----------------------------------------------------------
// Funções estáticas de callback específicas para cada relé
// Adicione mais/menos funções se `MAX_RELES` for maior/menor, 
// respeitando a variação do nome da função
//----------------------------------------------------------

void desligarRele0() { desligarRele(0); }
//void desligarRele1() { desligarRele(1); }

//---------------------------------------------------------
// Função para varrer agendamentos e programar os Ticker's
//---------------------------------------------------------
void verificarAgendamentos() 
{
    // Pega o timestamp corrente
    
    time_t now = time(nullptr);
    struct tm* currentTime = localtime(&now);

    // Formata o timestamp corrente
    
    char timestamp[30];
    strftime(timestamp, 30, "%d/%m/%Y %T", currentTime);
    Serial.printf("Verificando os agendamentos em %s\n", timestamp);

    // Formata a data corrente como YYYY-MM-DD
    
    char dataAtualStr[11]; // Espaço suficiente para "YYYY-MM-DD\0"
    strftime(dataAtualStr, sizeof(dataAtualStr), "%Y-%m-%d", currentTime);
    String dataAtualString = String(dataAtualStr);    
    
    int minutosAgora = currentTime->tm_hour * 60 + currentTime->tm_min;
    int diaAtual = currentTime->tm_wday; // 0 = Domingo, ..., 6 = Sábado

    // Faz a varredura na lista de agendamentos

    for (const auto& ag : dbAgenda) 
    {
        // Verifica primeiramente se atende a DataBase

        if (!verificarCondicaoData(ag.condicao, dataAtualString, ag.dataBase)) continue;

        // Verifica os demais critérios: dia da semana, repetição e intervalo de tempo
      
        if (std::find(ag.diaSemana.begin(), ag.diaSemana.end(), diaAtual) != ag.diaSemana.end()) 
        {
            int minutosInicial = horaParaMinutos(ag.horaInicial);
            int minutosFinal = horaParaMinutos(ag.horaFinal);
            int duracao = minutosFinal - minutosInicial;

            // Ajusta os horários de início e final considerando a repetição
            if (ag.repetirCadaMinutos > 0) 
            {
                while (minutosInicial + ag.repetirCadaMinutos <= minutosAgora) 
                {
                    minutosInicial += ag.repetirCadaMinutos;
                }
                // Garante que o ciclo anterior seja usado se já passou do atual
                if (minutosInicial > minutosAgora) 
                {
                    minutosInicial -= ag.repetirCadaMinutos;
                }
                minutosFinal = minutosInicial + duracao;
            }

            // Verifica se está dentro do intervalo de tempo do agendamento (original ou ajustado)
            
            if (minutosAgora >= minutosInicial && minutosAgora < minutosFinal) 
            {
                if (!statusRele[ag.releSelecionado]) 
                {
                    digitalWrite(pinRele[ag.releSelecionado], RELE_ON);
                    //statusRele[ag.releSelecionado] = true;
                    Serial.printf("Relé %d ligado.\n", ag.releSelecionado + 1);
                    //enviarStatusParaTodosClientes();
                    alterarEstadoRele(ag.releSelecionado,true);

                    int tempoRestante = (minutosFinal - minutosAgora) * 60; // Em segundos

                    // Use callbacks específicos para cada relé
                    // Ajuste o switch caso MAX_RELES for diferente de 2  
                    
                    switch (ag.releSelecionado) 
                    {
                        case 0:
                            timerReles[0].once(tempoRestante, desligarRele0);
                            break;
                        // case 1:
                        //     timerReles[1].once(tempoRestante, desligarRele1);
                        //     break;
                        // Adicione mais casos se MAX_RELES for maior
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------
// Função de comparação entre a data de processamento e a DataBase
// de acordo com o critério de comparação definido no agendamento
//-----------------------------------------------------------------

bool verificarCondicaoData(const String& condicao, const String& dataAtualStr, const String& dataBaseStr) 
{
    if (condicao == "<") return dataAtualStr < dataBaseStr;
    if (condicao == "<=") return dataAtualStr <= dataBaseStr;
    if (condicao == "=") return dataAtualStr == dataBaseStr;
    if (condicao == "!=") return dataAtualStr != dataBaseStr;
    if (condicao == ">") return dataAtualStr > dataBaseStr;
    if (condicao == ">=") return dataAtualStr >= dataBaseStr;
    return false; // Caso o operador não seja reconhecido
}

//------------------------------------------------
// Devolve o localtime dd/mm/aaaa hh:mm:ss
//------------------------------------------------

String getTimeStamp()
{
  time_t now;
  time(&now);
  char timestamp[30];
  strftime(timestamp, 30, "%d/%m/%Y %T", localtime(&now));
  return String(timestamp);
}

//---------------------------------------------------------
// Sincroniza o horário do ESP32 com  NTP server brasileiro
//---------------------------------------------------------

bool getNTPtime(int sec) 
{

  {
    uint32_t start = millis();

    tm timeinfo;
    time_t now;
    int cont=0;

    do 
    {
      time(&now);
      localtime_r(&now, &timeinfo);
      if (++cont % 80 == 0) Serial.println();
      else Serial.print(".");
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    Serial.print("\nnow ");
    Serial.println(now);
    Serial.print("Time "); 
    Serial.println(getTimeStamp());
  }

  return true;
}

//-------------------------------------------------------
// Define o HostName como DNS NAME
//-------------------------------------------------------

bool setDNSNAME(String nome)
{
   WiFi.setHostname(nome.c_str());
   delay(500);  // Dá tempo para estabilizar a rede antes de iniciar o mDNS   
   bool ok=false;

  if (ok=MDNS.begin(nome.c_str())) 
  {
      MDNS.addService("http", "tcp", 80);
      //MDNS.setInstanceName("esp8266"); // Adicionar o nome da instância      
  }

   return ok;
}

//------------------------------------------------
// Persiste NTP Server, Timezone e OTA no SPIFFS
//------------------------------------------------

String saveConfigFile()
// O arquivo de Config é salvo no formato JSON
{
  Serial.println(F("Persistindo a configuração..."));
  
  String result = "";

  // Atualiza a base de software e parâmetros gerais

  dbParm["ssid"] = ssid_wifi;
  dbParm["senhaSSID"] = pass_wifi;
  dbParm["apid"] = ssid_config;
  dbParm["senhaAP"] = pass_AP;
  dbParm["DnsName"] = DNS_NAME;  
  dbParm["NTPServer"] = NTP_SERVER;
  dbParm["Timezone"] = TZ_INFO;
  dbParm["intervaloNTP"] = intervaloNTP;
  dbParm["usuarioOTA"] = user_OTA;
  dbParm["senhaOTA"] = pass_OTA;
  dbParm["autorebootOTA"] = autoRebootOTA;
  dbParm["resetAgendamentos"] = reset_agendamentos;
  dbParm["modoOperacao"] = modo_operacao;

  // Abre o arquivo de configuração
  File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");
  if (!configFile)
  {
    // Erro, arquino não foi aberto
    result = "Erro ao abrir o arquivo para gravação da configuração";
    Serial.println(result);
    return result;
  }
 
  // Serializa os dados do JSON no arquivo
  serializeJsonPretty(dbParm, Serial);
  Serial.println();
  if (serializeJson(dbParm, configFile) == 0)
  {
    // Erro ai gravar o arquivo
    result = "Erro ao gravar o arquivo de configuração";
    Serial.println(result);
    return result;
  }
  // Fecha o Arquivo
  configFile.close();
  return String();
}

//------------------------------------------------
// Recupera NTP Server, Timezone e OTA do SPIFFS
//------------------------------------------------
 
bool loadConfigFile()
// Carrega o arquivo de Configuração
{
  // Verifica se o SPIFFS já foi inicializado
   
  if (!SPIFFS.begin())
  {
     SPIFFS.format();
     Serial.println("Sistema de Arquivo no SPIFFS foi formatado");
  }
 
  // Lê as configurações no formato JSON
  Serial.println("Montando o FileSystem...");
 
  // Força a entrada na primeira vez

  if (SPIFFS.begin()) 
  {
    Serial.println("FileSystem montado...");
    
    //Serial.println("Removendo o arquivo de configuração...");
    //SPIFFS.remove(JSON_CONFIG_FILE);
    
    if (SPIFFS.exists(JSON_CONFIG_FILE))
    {
      // o arquivo existe, vamos ler
      Serial.println("Lendo o arquivo de configuração");
      File configFile = SPIFFS.open(JSON_CONFIG_FILE, "r");
      if (configFile)
      {
        Serial.println("Arquivo de configuração aberto...");
        DeserializationError error = deserializeJson(dbParm, configFile);
        
        if (!error)
        {
          Serial.println("JSON do SPIFFS recuperado...");
          serializeJsonPretty(dbParm, Serial);
          Serial.println();          

          if (dbParm.containsKey("ssid")) ssid_wifi = dbParm["ssid"].as<String>();
          else ssid_wifi = "";
          
          if (dbParm.containsKey("senhaSSID")) pass_wifi = dbParm["senhaSSID"].as<String>();
          else pass_wifi = "";

          if (dbParm.containsKey("apid")) ssid_config = dbParm["apid"].as<String>();
          else ssid_config = getDefaultID(); 

          if (dbParm.containsKey("senhaAP")) pass_AP = dbParm["senhaAP"].as<String>();
          else pass_AP = DEFAULT_PASS_AP;          

          if (dbParm.containsKey("DnsName")) DNS_NAME = dbParm["DnsName"].as<String>();
          else DNS_NAME = DEFAULT_DNS_NAME;          
 
          if (dbParm.containsKey("NTPServer")) NTP_SERVER = dbParm["NTPServer"].as<String>();
          else NTP_SERVER = DEFAULT_NTP_SERVER;
          
          if (dbParm.containsKey("Timezone")) TZ_INFO = dbParm["Timezone"].as<String>();
          else TZ_INFO = DEFAULT_TZ_INFO;

          if (dbParm.containsKey("intervaloNTP")) intervaloNTP = dbParm["intervaloNTP"].as<int>();
          
          if (dbParm.containsKey("usuarioOTA")) user_OTA = dbParm["usuarioOTA"].as<String>();
          else user_OTA = USER_UPDATE;
          
          if (dbParm.containsKey("senhaOTA")) pass_OTA = dbParm["senhaOTA"].as<String>();
          else pass_OTA = PASS_UPDATE;

          if (dbParm.containsKey("autorebootOTA")) autoRebootOTA = dbParm["autorebootOTA"].as<bool>();
          else autoRebootOTA = false;

          if (dbParm.containsKey("resetAgendamentos")) reset_agendamentos = dbParm["resetAgendamentos"].as<bool>();
          else reset_agendamentos = false;

          if (dbParm.containsKey("modoOperacao")) modo_operacao = dbParm["modoOperacao"].as<int>();
          else modo_operacao = 0;                  

          return true;
        }
        else
        {
          // Erro ao ler o JSON
          Serial.println("Erro ao carregar o JSON da configuração...");
        }
      }
    }
  }
  else
  {
    // Erro ao montar o FileSystem
    Serial.println("Erro ao montar o FileSystem");
  }
 
  return false;
}
 
//----------------------------------------------------------
// Função de fazer o sincronismo do relógio interno com o
// servidor NTP definido nos paâmetros do WifiManage
//----------------------------------------------------------

void refreshNTPServer()
{
   // Sincroniza o horário interno com o Servidor NTP nacional

   Serial.print("Tentando sincronismo com o servidor NTP ");
   Serial.print(NTP_SERVER.c_str());
   Serial.print(" com TimeZone ");
   Serial.println(TZ_INFO.c_str());   
   
   // Verifica se está navegando pela internet pois às vezes fica conectado no AP porém sem internet

   if (!Ping.ping(ip,4))
   {
       Serial.println("Sem internet no momento...");
   }
   else 
   {     
      Serial.print("Internet ativa com média de ");
      Serial.print(Ping.averageTime());
      Serial.println(" ms");

      configTime(0, 0, NTP_SERVER.c_str());
      setenv("TZ", TZ_INFO.c_str(), 1);
      tzset();
    
      if (getNTPtime(10))
      { // wait up to 10sec to sync
        Serial.println("NTP Server sincronizado");
        sincronizouNTP = true;
      } 
      else 
      {
         Serial.println("Timer interno não foi sincronizado");
         //ESP.restart();
      }
   }
}

//------------------------------------------------
// Verifica se há autenticação na conexão
//------------------------------------------------

bool isAuthenticated(AsyncWebServerRequest *request) 
{
   if (!request->authenticate(user_OTA.c_str(), pass_OTA.c_str())) {
      request->requestAuthentication();
      return false;
   }
   return true;
}

String getDefaultID()
{
    // Monta o SSID do modo AP para permitir a configuração
  
    char aux[50] = "";
    sprintf(aux, "ESP8266_%X", ESP.getChipId());
    return String(aux);  
}

//---------------------
// Faz o reboot do ESP
//---------------------

void rebootESP() 
{
    ESP.restart();
}

//----------------------------
// Inicializa os Agendamentos
//----------------------------

void initAgendamentos()
{
    // Mostra Informações do Startup na Console
  
    Serial.printf("Horário Local do Startup: %s\n",getTimeStamp().c_str());
    Serial.print("Servidor iniciado no IP ");
    if (modo_operacao==0) Serial.println(WiFi.softAPIP());
    else Serial.println(WiFi.localIP().toString());
    
    // Agendamento periódico para verificar agendamentos a cada minuto
    // Calcula o tempo até o próximo minuto com segundos zerados
    
    time_t now = time(nullptr);
    struct tm* currentTime = localtime(&now);
    int segundosRestantes = 60 - currentTime->tm_sec;

    // Programa a primeira execução de verificarAgendamentos()

    verificaAgendamentosTimer.once(segundosRestantes, []() {
        verificarAgendamentos();
        // Programa a execução periódica a cada 60 segundos após a primeira execução
        verificaAgendamentosTimer.attach(60, verificarAgendamentos);
    });

    // Avisa sobre a primeira varredura dos Agendamentos

    Serial.printf("Primeira verificação programada para %d segundos.\n", segundosRestantes);    

}

//-------------------------
// Envia retorno de Evento
//-------------------------

void notifyClients(String event, String status, String message) 
{
    StaticJsonDocument<100> jsonDoc;
    jsonDoc["event"] = event;
    jsonDoc["status"] = status;

    if (status.equalsIgnoreCase("ok")) jsonDoc["message"] = "✅ " + message;
    else jsonDoc["message"] = "❌ " + message;

    String response;
    serializeJson(jsonDoc, response);
    //serializeJsonPretty(jsonDoc, Serial);    
    ws.textAll(response);
}