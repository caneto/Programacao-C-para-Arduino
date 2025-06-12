// Carregar biblioteca Wi-Fi
#include <ESP8266WiFi.h>

// Substitua por suas credenciais de rede
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Defina o número da porta do servidor da Web como 80
WiFiServer server(80);

// Decodificar o valor HTTP GET
String redString = "0";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

// Variável para armazenar a solicitação HTTP
String header;

// Pinos vermelho, verde e azul para controle de PWM
const int redPin = 13;     // 13 corresponds to GPIO13
const int greenPin = 12;   // 12 corresponds to GPIO12
const int bluePin = 14;    // 14 corresponds to GPIO14

// Configuração da resolução de bits PWM
const int resolution = 256;

// Hora atual
unsigned long currentTime = millis();
// Tempo anterior
unsigned long previousTime = 0; 
// Defina o tempo limite em milissegundos (exemplo: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  // configurar a resolução/intervalo do PWM do LED e definir os pinos como LOW
  analogWriteRange(resolution);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  
  // Conecte-se à rede Wi-Fi com SSID e senha
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprimir o endereço IP local e iniciar o servidor da Web
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Ouvir os clientes que estão chegando

  if (client) {                             // Se um novo cliente se conectar,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // criar um String para armazenar os dados recebidos do cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop enquanto o cliente estiver conectado
      currentTime = millis();
      if (client.available()) {             // se houver bytes para ler do cliente,
        char c = client.read();             // ler um byte, então
        Serial.write(c);                    // imprimi-lo no monitor serial
        header += c;
        if (c == '\n') {                    // se o byte for um caractere de nova linha
          // se a linha atual estiver em branco, você tem dois caracteres de nova linha em uma linha.
          // Esse é o fim da solicitação HTTP do cliente, portanto, envie uma resposta:
          if (currentLine.length() == 0) {
            // Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP/1.1 200 OK)
            // e um tipo de conteúdo para que o cliente saiba o que está por vir e, em seguida, uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                   
            // Exibir a página da Web em HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
            client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
            client.println("</head><body><div class=\"container\"><div class=\"row\"><h1>ESP Color Picker</h1></div>");
            client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");
            client.println("<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div>");
            client.println("<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
            client.println("document.getElementById(\"change_color\").href=\"?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}</script></body></html>");
            // A resposta HTTP termina com outra linha em branco
            client.println();

            // Exemplo de solicitação: /?r201g32b255&
            // Vermelho = 201 | Verde = 32 | Azul = 255
            if(header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
              /*Serial.println(redString.toInt());
              Serial.println(greenString.toInt());
              Serial.println(blueString.toInt());*/
              analogWrite(redPin, redString.toInt());
              analogWrite(greenPin, greenString.toInt());
              analogWrite(bluePin, blueString.toInt());
            }
            // Sair do loop while
            break;
          } else { // se você tiver uma nova linha, limpe a linha atual
            currentLine = "";
          }
        } else if (c != '\r') {  // se você tiver qualquer outra coisa que não seja um caractere de retorno de carro,
          currentLine += c;      // adicione-o ao final da currentLine
        }
      }
    }
    // Limpar a variável de cabeçalho
    header = "";
    // Fechar a conexão
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}