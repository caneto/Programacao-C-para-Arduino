//-----------------------------------------------------------------------------------------
// Função   : Este programa tem como objetivo implementar a programação de horários para
//            acionamento e desligamento de relés, com suporte a operações manuais e 
//            automáticas através de uma interface HTML numa aplicação AsyncWebServer com 
//            para ESP32 com interação com o Navegador via WebSocket.
//
// Funções adicionais
//
//   1) Inclusão de Alias no DNS da Rede Local (mDNS)
//   2) Sincronização do Relógio interno com o Serviço NTP 
//   3) Atualização do Código via Interface Web (ElegantOTA)
//   4) Modo de Configuração dos Parãmetros (WiFiManager)
//   5) Generalização para mais ou menos Relés (MAX_RELES) com poucos ajustes no CALLBACK
//      Veja as instruções em alguns pontos do código  
//   6) Utilização de Ticker para evitar TIMER de hardware que são mais restritos em quantidade 
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
// Versão    : 1.0 Nov/2024 
//
// Atualização
// Autor     : Carlos Aberto
// Versão    : 1.1 Mai/2025
//-----------------------------------------------------------------------------------------
// Patch no src do ElegantOta
// 1) Vá para o diretório de bibliotecas do Arduino.
// 2) Abra a pasta ElegantOTA e depois abra a pasta src.
// 3) Abra o arquivo ElegantOTA.h. Pode abrir com o bloco de notas.
// 4) Com o arquivo aberto procure a linha: #define ELEGANTOTA_USE_ASYNC_WEBSERVER 0
// 5) Mude o valor no final da linha de 0 para 1
// 6) Salve e feche o arquivo e compile o programa novamente.
//-----------------------------------------------------------------------------------------
// ---> Apenas na versão do Esp32 2.0.17 (As superiors dão erro)

//--------------------------------------
// Definição das Bibliotecas utilizadas
//--------------------------------------

#include <WiFi.h>                            // Biblioteca para a rede wifi --> Usar  esp32 versão 2.0.17 (As atuais da erro)
#include <AsyncTCP.h>                        // Biblioteca usada pelo Servidor Assíncrono
#include <ESP32Ping.h>                       // Biblioteca Ping
#include <FS.h>                              // Biblioteca para manipular arquivos no filesystem
#include <SPIFFS.h>                          // Biblioteca que implementa o filesystem  
#include <WiFiManager.h>                     // Biblioteca WiFi Manager        
#include <ESPAsyncWebServer.h>               // Biblioteca para Servidor Web Assíncrono ---> Versão 3.6.0
#include <ESPmDNS.h>                         // Biblioteca para adionaar aliases no DNS da Rede Local 
#include <ArduinoJson.h>                     // Biblioteca para manipulação de estrutiras JSON
#include <Ticker.h>                          // Biblioteca para programaçao de eventos
#include <vector>                            // Biblioteca para manipulação de vetores 
#include <ctime>                             // Biblioteca para manipulação de tempo
#include <ElegantOTA.h>                      // Biblioteca para atualização via Web ---> Versão 3.1.1

//----------------------------
// Definições para o programa
//----------------------------

#define MAX_RELES            2               // Números de Relés
#define pinRELE1             25              // Porta para o Rele 1
#define pinRELE2             26              // Porta para o Rede 2
#define pinBoot              0               // Pino do botão para forçar a entrada no WifiManager  
#define RELE_ON              LOW             // Estado para o Relé Ativo
#define RELE_OFF             HIGH            // Estado para Relé desativado
#define DEFAULT_DNS_NAME     "relaytimer"    // Nome para adicionar no mDNS
#define LED_BUILTIN          2               // Pino do Led BuiltIn interno para indicar Wifi ON/OFF
#define JSON_AGENDA_FILE     "/agenda.json"  // Arquivo JSON para os Agendamentos
#define JSON_CONFIG_FILE     "/config.json"  // Arquivo JSON de configuração
#define BAUDRATE             115200          // Baudrate para a Console
#define MAX_EDIT_LEN         30              // Tamanho máximo de campos de EDIT
#define MAX_NUM_LEN          4               // Tamanho máximo de campos NUMÉRICO
#define ESP_getChipId()      ((uint32_t)ESP.getEfuseMac() // Simular ID da placa ESP
#define USER_UPDATE          "admin"         // Usuário para atualização via OTA
#define PASS_UPDATE          "esp32@agenda"  // Senha para atualização via OTA
#define DEFAULT_PASS_AP      "12345678"      // Senha default do modo AP WifiManager
#define DEFAULT_NTP_SERVER   "a.st1.ntp.br"  // Servidor NTP do Brasil
#define DEFAULT_TZ_INFO      "<-03>3"        // TimeZone do Brasil
#define DEFAULT_REFRESH_NTP  720             // Intervalo para Refresh do Horário NTP Server (min)

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
int pinRele[MAX_RELES] = {pinRELE1, pinRELE2};// Vetor de Pinos dos Relés
bool statusRele[MAX_RELES] = {false};        // Vetor de Estado dos Relés
Ticker timerReles[MAX_RELES];                // Vetor de Timers dos Relés
Ticker verificaAgendamentosTimer;            // Timer para varredura do agendamento 
Ticker refreshNTPTimer;                      // Timer para resincronização do horário com o NTP Server 
volatile bool buttonState = false;           // Estado do botão Boot para Reconfiguração do WiFi
IPAddress ip (1, 1, 1, 1);                   // The remote ip to ping, DNS do Google
JsonDocument dbParm;                         // Base de dados de parâmetros

//---------------------------------------------
// Variáveis para controle do OTA
//---------------------------------------------

bool autoRebootOTA     = true;                                   // Se deve fazer autoreboot após a atualização OTA
char user_OTA[MAX_EDIT_LEN] = USER_UPDATE;                       // Usuário para atualização OTA
char pass_OTA[MAX_EDIT_LEN] = PASS_UPDATE;                       // Senha para atualização OTA
char val_autoreboot[2] = "1";                                    // AutoRebbot Default 

//---------------------------------------------
// Variáveis para controle do WifiManger/OTA
//---------------------------------------------

WiFiManager wm;                                                  // Define o Objeto WiFiManager
bool shouldSaveConfig = false;                                   // Flag se deve persistir os parãmetros
char NTP_SERVER[MAX_EDIT_LEN+1] = DEFAULT_NTP_SERVER;            // Servidor NTP
char TZ_INFO[MAX_EDIT_LEN+1]    = DEFAULT_TZ_INFO;               // String do TimeZone 
char DNS_NAME[MAX_EDIT_LEN+1]   = DEFAULT_DNS_NAME;              // Nome Default para o DNS
char ssid_config[MAX_EDIT_LEN+1];                                // SSID para o modo  AP de Configuração 
char pass_config[] = DEFAULT_PASS_AP;                            // Senha para o modo AP de Configuração
char valIntervaloNTP[MAX_NUM_LEN+1]= "720";                      // Intervalo NTP 
int intervaloNTP = DEFAULT_REFRESH_NTP;                          // Para receber o Intervalo NTP (min)
char val_reset_agendamentos[2] = "0";                            // Reset Agendamentos Char
bool reset_agendamentos = false;                                 // Reset Agenda,entos Bool
WiFiManagerParameter custom_dnsname("DnsName", "Informe o DNSNAME (< 30)", DNS_NAME, MAX_EDIT_LEN);           // Parâmetro NTP Server
WiFiManagerParameter custom_ntpserver("NTPServer", "Informe o NTP Server (< 30)", NTP_SERVER, MAX_EDIT_LEN);           // Parâmetro NTP Server
WiFiManagerParameter custom_timezone("Timezone", "Informe o String Timezone (< 30)", TZ_INFO, MAX_EDIT_LEN);           // Parâmetro Timezone 
WiFiManagerParameter custom_intervalo_ntp("IntervaloNTP", "Informe o Intervalo de Refresh do NTP Server (< 1440 min)", valIntervaloNTP, MAX_NUM_LEN); // Parâmetro Intervalo Refresh NTP 
WiFiManagerParameter custom_user_ota("Usuario", "Informe o Usuário para Atualizações (< 15)", user_OTA, MAX_EDIT_LEN); // Parâmetro Nome  do Usuário OTA
WiFiManagerParameter custom_pass_ota("Senha", "Informe a Senha para Atualizações (< 15)", pass_OTA, MAX_EDIT_LEN);     // Parâmetro Senha do Usuário OTA
WiFiManagerParameter custom_autoreboot_ota("AutoReboot", "AutoReboot após Atualizações (0 ou 1)", val_autoreboot, 1);  // Parâmetro AutoRebbot
WiFiManagerParameter custom_reset_agendamentos("ResetAgendamentos", "Reset Agendamentos (0 ou 1)", val_reset_agendamentos, 1);  // Parâmetro Reset Agendamentos

//--------------------------------------
// Define o JSON Default dos Parâmetros
//--------------------------------------

const char dbDefault[] PROGMEM = R"(
{
   "DnsName": "relaytimer",
   "NTPServer": "a.st1.ntp.br",
   "Timezone": "<-03>3", 
   "intervaloNTP": "720",
   "usuarioOTA": "admin",
   "senhaOTA": "esp32@agenda",
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
    <title>Controle de Relés</title>
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

        window.onload = function() {
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
        };

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

        //setInterval(atualizarStatus, 5000);
    </script>
</head>
<body>

    <header>
        <h3>Painel de Controle de Relés</h3>
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
</body>
</html>
)rawliteral";

//--------------------------------
// Prototipação das funções usadas
//--------------------------------

// Wifi

void WiFiEvent(WiFiEvent_t event);                   // Trata os eventos de Wifi 

// WebSocket

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len); // Trata eventos WebSocket

// WifiManager

void Check_WiFiManager(bool forceConfig);            // Inicialização/Configuração  WiFi Manager no ESP32
void saveConfigFile();                               // Persiste CPUID e Intervalo no SPIFFS do ESP32
bool loadConfigFile();                               // Recupera CPUID e Intervalo do SPIFFS do ESP32
void saveConfigCallback();                           // Callback para informação do processo de configuração WiFi
void configModeCallback(WiFiManager *myWiFiManager); // Callback para WifiManager 
void buttonISR();                                    // Rotina de Tratamento da Interrupção do Botão Boot

// Auxiliares

bool getNTPtime(int sec);                            // Faz o sincronismo do relógio com  Servidor NTP
String getTimeStamp();                               // Retorna o TimeStamp (Data/Hora)            
String expandeHtml(String html);                     // Expande o HTML para o Navegador
String repeatChar(char c, int num);                  // Devolve um string com a repetição de um determinado carater
void deleteFile(const char* path);                   // Apaga o arquivo de Agendamentos no SPIFFS (uso esporádico)
int horaParaMinutos(String hora);                    // Converte hh:mm para total em minutos
bool setDNSNAME(String nome);                        // Define o DNSNAME e HOSTNAME da Rede 
void refreshNTPServer();                             // Faz  o sincronismo com Servidro NTP

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

//-------------------------------------------
// Prototipação das rotinas de CALLBACK
// Ajuste caso MAX_RELES seja diferente de 2
//-------------------------------------------

void desligarRele0();                                // Callback de desligamento para o relé 1 
void desligarRele1();                                // Callback de desligamento para o Relé 2

//------------------------
// Configurações iniciais
//------------------------

void setup() 
{
    // Inicializa a Serial
    
    Serial.begin(BAUDRATE);
    while (!Serial);

    // Define o CALLBACK do modo CONFIG com alteração
  
    wm.setSaveConfigCallback(saveConfigCallback);
 
    // Define o CALLBACK do modo CONFIG
  
    wm.setAPCallback(configModeCallback);
 
    // Adiciona os campos de parâmetros no MENU do WifiManager

    wm.addParameter(&custom_dnsname);
    wm.addParameter(&custom_ntpserver);
    wm.addParameter(&custom_timezone); 
    wm.addParameter(&custom_intervalo_ntp); 
    wm.addParameter(&custom_user_ota);    
    wm.addParameter(&custom_pass_ota); 
    wm.addParameter(&custom_autoreboot_ota);
    wm.addParameter(&custom_reset_agendamentos);

    // Define o handle para tratar os eventos do Wifi

    WiFi.onEvent(WiFiEvent);   
   
    // Define o Led conectado no WiFi ou desligado quando fora 

    pinMode(LED_BUILTIN,OUTPUT);  

    // Define os Relés e inicializa

    for (int ind=0; ind < MAX_RELES; ind++)
    {
       pinMode(pinRele[ind], OUTPUT);
       digitalWrite(pinRele[ind], RELE_OFF);
       statusRele[ind] = false;
    }    

    // Inicializa o Botão interno do ESP32

    pinMode(pinBoot, INPUT_PULLUP);

    // Configura a interrupção para detectar a borda de descida do botão Boot

    attachInterrupt(digitalPinToInterrupt(pinBoot), buttonISR, FALLING); 
  
    // Defina a porta do WiFiManager para 8080 no modo AP para não conflitar com a
    // porta 80 que vamos utilizar para responder as requisições
  
    wm.setHttpPort(8080);
  
    // Chama Wifi_Manager para conectar no Wifi ou entrar em modo de configuração
    // caso os parãmetros SSID, Senha, CPIID e Intervalo do TIMER não estejam persistidos

    Check_WiFiManager(!wm.getWiFiIsSaved());
    
    // Verifica se teve está conectado na Internet

    if (WiFi.status() == WL_CONNECTED)
    { 
       // Se chegamos até aqui é porque estamos conectados
  
       Serial.println("WiFi conectado...");
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
   
    ElegantOTA.setAuth(user_OTA,pass_OTA);

    // Habilita/Desabilita AutoRebbot após a atualização
   
    ElegantOTA.setAutoReboot(autoRebootOTA);   

    // Inicia o OTA para atualização via Web

    ElegantOTA.begin(&server);           

    // Inicializa a rota para o HTML

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", expandeHtml(index_html));
    });

    // Inicializa o Servidor Web

    server.begin();

    // Monta o SSID do modo AP para permitir a configuração
  
    sprintf(ssid_config, "ESP32_%X",(uint32_t)ESP.getEfuseMac());      

    // Mostra Informações do Startup na Console
   
    Serial.printf("Horário Local do Startup: %s\n",getTimeStamp().c_str());
    Serial.printf("Servidor iniciado no IP %s\n",WiFi.localIP().toString());
    
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

    // Programa a resincronização com o NTP Server de acordo com o intervalo definido no WifiManager

    refreshNTPTimer.attach(intervaloNTP*60, refreshNTPServer);

    // Avisa sobre a primeira varredura dos Agendamentos

    Serial.printf("Primeira verificação programada para %d segundos.\n", segundosRestantes);
}

//----------------------------
// Loop principal do Programa
//----------------------------

void loop() 
{
   //----------------------------------------------------------
   // Função 1 : Limpa as conexões websocket perdidas por algum 
   //            motivo (ex: Navegador fechado)
   //-----------------------------------------------------------
     
    ws.cleanupClients();

   //--------------------------------------------------------------------------------------------------
   // Bloco 2 : Verifica se o botão de BOOT foi apertado para forçar a entrada no modo de configuração. 
   //           É útil quando a senha do wifi mudou ou está se conectando em outra rede wifi. Isso 
   //           evita ter o SSID/senha no código, a recompilação e upload do código no ESP32.
   //--------------------------------------------------------------------------------------------------

   if (buttonState)
   {
      // Reseta o estado do botão

      buttonState = false;

      // Força a entrada em modo de configuração

      wm.resetSettings();   
      ESP.restart();
       
   }

   //--------------------------------------------------------------
   // Função 3 : checa o OTA para saber se há atualização
   //--------------------------------------------------------------  

   ElegantOTA.loop();       
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
                    serializeJsonPretty(doc, Serial);
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

void WiFiEvent(WiFiEvent_t event)
{
  Serial.printf("[Evento Wi-Fi] evento: %d\n", event);

  switch (event)
  {
    case SYSTEM_EVENT_WIFI_READY:
      Serial.println("interface WiFi pronta");
      break;
    case SYSTEM_EVENT_SCAN_DONE:
      Serial.println("Pesquisa por AP completada");
      break;
    case SYSTEM_EVENT_STA_START:
      Serial.println("Cliente WiFi iniciado");
      break;
    case SYSTEM_EVENT_STA_STOP:
      Serial.println("Clientes WiFi  cancelados");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("Conectado ao AP");
      digitalWrite(LED_BUILTIN,HIGH);   // Liga o LED 
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("Desconectado do AP WiFi");
       digitalWrite(LED_BUILTIN,LOW); // Desliga o LED 
      //Check_WiFiManager(false);
      break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
      Serial.println("Modo de Autenticação do AP mudou");
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.print("Endereço IP obtido: ");
      Serial.println(WiFi.localIP());
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      Serial.println("Endereço IP perdido e foi resetado para 0");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      Serial.println("WPS: modo enrollee bem sucedido");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      Serial.println("WPS: modo enrollee falhou");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      Serial.println("WPS: timeout no modo enrollee");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      Serial.println("WPS: pin code no modo enrollee");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("AP Wifi Iniciado");
      break;
    case SYSTEM_EVENT_AP_STOP:
      Serial.println("AP Wifi parado");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Cliente conectado");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("Cliente desconectado");
      break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      Serial.println("IP associado ao Cliente");
      break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
      Serial.println("Requisição de probe recebida");
      break;
    case SYSTEM_EVENT_GOT_IP6:
      Serial.println("IPv6 é preferencial");
      break;
    case SYSTEM_EVENT_ETH_START:
      Serial.println("Interface Ethernet iniciada");
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("Interface Ethernet parada");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("Interface Ethernet conectada");
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("Interface Ethernet desconectada");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.println("Endereço IP obtido");
      break;
    default: break;
  }
}

//----------------------------------------------
// Função para tratar as mensagens do WebSocket
//----------------------------------------------

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_DATA) {
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

                    Serial.printf("Evento: updateAgenda idEvento: %s updated: %d\n",ag.idEvento.c_str(),updated);

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
                    Serial.printf("Evento: deleteAgenda idEvento: %s \n",idEvento.c_str());
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
        script += espacos + "document.getElementById('statusRele" + String(ind + 1) + "').textContent = response.rele" + String(ind + 1) + " ? 'Ligado' : 'Desligado';\n";
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
void desligarRele1() { desligarRele(1); }

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
                        case 1:
                            timerReles[1].once(tempoRestante, desligarRele1);
                            break;
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
   bool ok = MDNS.begin(nome.c_str());
   if (ok) 
   {
      MDNS.addService("http", "tcp", 80);
      MDNS.setInstanceName(nome.c_str()); // Adicionar o nome da instância      
   }
   return ok;
}

//--------------------------------------------------
// Rotina de Tratamento da Interrupção do Botão Boot
//--------------------------------------------------

void buttonISR() 
{
   buttonState = true;
}

//------------------------------------------------
// Persiste NTP Server, Timezone e OTA no SPIFFS
//------------------------------------------------

void saveConfigFile()
// O arquivo de Config é salvo no formato JSON
{
  Serial.println(F("Persistindo a configuração..."));
  
  // Atualiza a base de software e parâmetros gerais

  dbParm["DnsName"] = DNS_NAME;  
  dbParm["NTPServer"] = NTP_SERVER;
  dbParm["Timezone"] = TZ_INFO;
  dbParm["intervaloNTP"] = intervaloNTP;
  dbParm["usuarioOTA"] = user_OTA;
  dbParm["senhaOTA"] = pass_OTA;
  dbParm["autorebootOTA"] = autoRebootOTA;
  dbParm["resetAgendamentos"] = reset_agendamentos;

  // Abre o arquivo de configuração
  File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");
  if (!configFile)
  {
    // Erro, arquino não foi aberto
    Serial.println("Erro ao persistir a configuração");
  }
 
  // Serializa os dados do JSON no arquivo
  serializeJsonPretty(dbParm, Serial);
  Serial.println();
  if (serializeJson(dbParm, configFile) == 0)
  {
    // Erro ai gravar o arquivo
    Serial.println(F("Erro ao gravar o arquivo de configuração"));
  }
  // Fecha o Arquivo
  configFile.close();
}

//------------------------------------------------
// Recupera NTP Server, Timezone e OTA do SPIFFS
//------------------------------------------------
 
bool loadConfigFile()
// Carrega o arquivo de Configuração
{

  // Verifica se o SPIFFS já foi inicializado
  
  if (!SPIFFS.begin(true))
  {
     SPIFFS.format();
     Serial.println("Sistema de Arquivo no SPIFFS foi formatado");
  }
 
  // Lê as configurações no formato JSON
  Serial.println("Montando o FileSystem...");
 
  // Força a entrada na primeira vez
  if (SPIFFS.begin(true))
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

          if (dbParm.containsKey("DnsName")) strcpy(DNS_NAME, dbParm["DnsName"]);
          else strcpy(DNS_NAME, DEFAULT_DNS_NAME);          
 
          if (dbParm.containsKey("NTPServer")) strcpy(NTP_SERVER, dbParm["NTPServer"]);
          else strcpy(NTP_SERVER, DEFAULT_NTP_SERVER);
          
          if (dbParm.containsKey("Timezone")) strcpy(TZ_INFO, dbParm["Timezone"]);
          else strcpy(TZ_INFO, DEFAULT_TZ_INFO);

          if (dbParm.containsKey("intervaloNTP")) intervaloNTP = dbParm["intervaloNTP"].as<int>();
          else 
          {
             intervaloNTP = 720;
             strcpy(valIntervaloNTP,"720");
          }          
          
          if (dbParm.containsKey("usuarioOTA")) strcpy(user_OTA, dbParm["usuarioOTA"]);
          else strcpy(user_OTA, USER_UPDATE);
          
          if (dbParm.containsKey("senhaOTA")) strcpy(pass_OTA, dbParm["senhaOTA"]);
          else strcpy(pass_OTA, PASS_UPDATE);

          if (dbParm.containsKey("autorebootOTA")) 
          {
             autoRebootOTA = dbParm["autorebootOTA"];
             if (autoRebootOTA) strcpy(val_autoreboot,"1");
             else strcpy(val_autoreboot,"0"); 
          }
          else 
          {
             autoRebootOTA = true;
             strcpy(val_autoreboot,"1");     
          }

          if (dbParm.containsKey("resetAgendamentos")) 
          {
             reset_agendamentos = dbParm["resetAgendamentos"];
             if (reset_agendamentos) strcpy(val_reset_agendamentos,"1");
             else strcpy(val_reset_agendamentos,"0"); 
          }
          else 
          {
             reset_agendamentos = true;
             strcpy(val_reset_agendamentos,"1");     
          }

          return true;
        }
        else
        {
          // Erro ao ler o JSON
          Serial.println("Erro ao carregar o JSON da configuração...");
        }
      }
    }
    else
    {
       // Monta base default   

       DeserializationError error = deserializeJson(dbParm, dbDefault);
    
       // Verificar se há erro no parsing
       
       if (!error)
       {
          Serial.println("JSON default recuperado...");
          serializeJsonPretty(dbParm, Serial);
          Serial.println();                 

          strcpy(DNS_NAME, dbParm["DnsName"]);
          strcpy(NTP_SERVER, dbParm["NTPServer"]);
          strcpy(TZ_INFO, dbParm["Timezone"]);
          intervaloNTP = dbParm["intervaloNTP"].as<int>();
          strcpy(valIntervaloNTP,dbParm["intervaloNTP"].as<String>().c_str());
          strcpy(user_OTA, dbParm["usuarioOTA"]);
          strcpy(pass_OTA, dbParm["senhaOTA"]);
          autoRebootOTA = dbParm["autorebootOTA"];
          if (autoRebootOTA) strcpy(val_autoreboot,"1");
          else strcpy(val_autoreboot,"0"); 
          reset_agendamentos = dbParm["resetAgendamentos"];
          if (reset_agendamentos) strcpy(val_reset_agendamentos,"1");
          else strcpy(val_reset_agendamentos,"0");          

          // Salva o default no SPIFFS

          saveConfigFile();

          return true;
       }
       else
       {
          // Erro ao ler o JSON
          Serial.println("Erro ao carregar o JSON da configuração...");
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

//----------------------------------------------------
// Inicialização/Configuração do WiFi Manager no ESP32
//----------------------------------------------------

void Check_WiFiManager(bool forceConfig)
{

   // Tenta carregar os parâmetros do SPIFFS
  
   bool spiffsSetup = loadConfigFile();
   if (!spiffsSetup)
   {
      Serial.println(F("Forçando o modo de configuração..."));
      forceConfig = true;
   }

   // Copia os campos para o FORM do WifiManager

   custom_dnsname.setValue(DNS_NAME, MAX_EDIT_LEN+1);
   custom_ntpserver.setValue(NTP_SERVER, MAX_EDIT_LEN+1);
   custom_timezone.setValue(TZ_INFO, MAX_EDIT_LEN+1); 
   custom_intervalo_ntp.setValue(String(intervaloNTP).c_str(), MAX_NUM_LEN+1); 
   custom_user_ota.setValue(user_OTA, MAX_EDIT_LEN+1); 
   custom_pass_ota.setValue(pass_OTA, MAX_EDIT_LEN+1);
   custom_autoreboot_ota.setValue(val_autoreboot,sizeof(val_autoreboot));
   custom_reset_agendamentos.setValue(val_reset_agendamentos,sizeof(val_reset_agendamentos));

   if (forceConfig) 
   { 
      // reseta configurações
      
      wm.resetSettings();   

      // Define o modo AP
  
      WiFi.mode(WIFI_STA);

      // Entra no modo de AP de configuração ... com senha fixa
     
      if (!wm.startConfigPortal(ssid_config, pass_config))
      {
         Serial.println("Erro na conexão com timeout no modo AP...");
         //setStateWifiEEPROM(true);
      }
      //else setStateWifiEEPROM(false);
    
   }
   else
   {
      // Entra no modo de conexão normal recuperando o SSID/Senha anteriores
      if (!wm.autoConnect())
      {
         Serial.println("Erro na conexão com timeout...");
      }
      //setStateWifiEEPROM(false);
   }

   // Recupera o campo DNSNAME preenchido na interface do WifiManager
  
   strncpy(DNS_NAME, custom_dnsname.getValue(), sizeof(DNS_NAME));
   Serial.print("DNSNAME:");
   Serial.println(DNS_NAME);

   // Recupera o campo NTP SERVER preenchido na interface do WifiManager
  
   strncpy(NTP_SERVER, custom_ntpserver.getValue(), sizeof(NTP_SERVER));
   Serial.print("NTP_SERVER:");
   Serial.println(NTP_SERVER);
 
   // Recupera o campo intervaloTimer do WifiManager preenchido na interface convertendo para inteiro

   strncpy(TZ_INFO, custom_timezone.getValue(), sizeof(TZ_INFO));
   Serial.print("TZ_INFO: ");
   Serial.println(TZ_INFO);

   // Recupera o campo intervaloNPT do WifiManager preenchido na interface convertendo para inteiro
  
   intervaloNTP = atoi(custom_intervalo_ntp.getValue());
   if (intervaloNTP < 60 || intervaloNTP > 1440) intervaloNTP = DEFAULT_REFRESH_NTP;
   Serial.print("intervaloNTP: ");
   Serial.println(intervaloNTP);   

   // Recupera o campo usuário da Atualização do WifiManager

   strncpy(user_OTA, custom_user_ota.getValue(), sizeof(user_OTA));
   Serial.print("User_OTA: ");
   Serial.println(user_OTA);

   // Recupera o campo senha da Atualização do WifiManager

   strncpy(pass_OTA, custom_pass_ota.getValue(), sizeof(pass_OTA));
   Serial.print("Pass_OTA: ");
   Serial.println(pass_OTA);

   // Recupera o campo AutoReboot da Atualização do WifiManager

   strncpy(val_autoreboot, custom_autoreboot_ota.getValue(), sizeof(val_autoreboot));
   Serial.print("AutoReboot_OTA: ");
   Serial.println(val_autoreboot);   
   autoRebootOTA = (strcmp(val_autoreboot, "1") == 0) ? true : false;

   // Recupera o campo resetAgendamentos da Atualização do WifiManager

   strncpy(val_reset_agendamentos, custom_reset_agendamentos.getValue(), sizeof(val_reset_agendamentos));
   Serial.print("ResetAgendamentos: ");
   Serial.println(val_reset_agendamentos);   
   reset_agendamentos = (strcmp(val_reset_agendamentos, "1") == 0) ? true : false;   

   // Salva os parâmetros no FileSystem FLASH -> não perde quando desligado
  
   if (shouldSaveConfig)
   {
      saveConfigFile();
   } 
  
}
 
//----------------------------------------------------------
// Callback para informação do processo de configuração WiFi
//----------------------------------------------------------
 
void saveConfigCallback()
// Callback para nos lembrar de salvar o arquivo de configuração
{
  Serial.println("Persistência necessária...");
  shouldSaveConfig = true;
}

//----------------------------------------------------------
// Callback para WifiManager 
//----------------------------------------------------------
 
void configModeCallback(WiFiManager *myWiFiManager)
// É chamado no modo de configuração
{
  Serial.println("Entrando no modo de configuração...");

  Serial.print("Config SSID: ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
 
  Serial.print("Config IP Address: ");
  Serial.println(WiFi.softAPIP());
}

//----------------------------------------------------------
// Função de fazer o sincronismo do relógio interno com o
// servidor NTP definido nos paâmetros do WifiManage
//----------------------------------------------------------

void refreshNTPServer()
{
   // Sincroniza o horário interno com o Servidor NTP nacional

   Serial.print("Tentando sincronismo com o servidor NTP ");
   Serial.print(NTP_SERVER);
   Serial.print(" com TimeZone ");
   Serial.println(TZ_INFO);   
   
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

      configTime(0, 0, NTP_SERVER);
      setenv("TZ", TZ_INFO, 1);
      tzset();
    
      if (getNTPtime(10))
      { // wait up to 10sec to sync
        Serial.println("NTP Server sincronizado");
      } 
      else 
      {
         Serial.println("Timer interno não foi sincronizado");
         //ESP.restart();
      }
   }
}