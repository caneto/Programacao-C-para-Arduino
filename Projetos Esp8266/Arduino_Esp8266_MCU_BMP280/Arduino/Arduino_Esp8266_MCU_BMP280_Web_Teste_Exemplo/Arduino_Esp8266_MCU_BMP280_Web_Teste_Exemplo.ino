// Carregar biblioteca Wi-Fi
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

//descomente as seguintes linhas se estiver usando SPI
/*#include <SPI.h>
#define BME_SCK 14
#define BME_MISO 12
#define BME_MOSI 13
#define BME_CS 15*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Substitua por suas credenciais de rede
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Definir o número da porta do servidor web como 80
WiFiServer server(80);

// Variável para armazenar a solicitação HTTP
String header;

void setup() {
  Serial.begin(115200);
  bool status;

  // configurações padrão
  // (você também pode passar um objeto da biblioteca Wire, como &Wire2)  //status = bme.begin();  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Conectar-se à rede Wi-Fi com SSID e senha
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
    Serial.println("New Client.");          // imprimir uma mensagem na porta serial
    String currentLine = "";                // criar um String para armazenar os dados recebidos do cliente
    while (client.connected()) {            // loop enquanto o cliente estiver conectado
      if (client.available()) {             // se houver bytes para ler do cliente,
        char c = client.read();             // ler um byte, então
        Serial.write(c);                    // imprimi-lo no monitor serial
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
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
            // CSS para estilizar a tabela
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
            client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #0043af; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
            // Título da página da Web
            client.println("</style></head><body><h1>ESP8266 with BME280</h1>");
            client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
            client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
            client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");  
            client.println("<tr><td>Temp. Fahrenheit</td><td><span class=\"sensor\">");
            client.println(1.8 * bme.readTemperature() + 32);
            client.println(" *F</span></td></tr>");       
            client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Approx. Altitude</td><td><span class=\"sensor\">");
            client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
            client.println(" m</span></td></tr>"); 
            client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>"); 
            client.println("</body></html>");
            
            // A resposta HTTP termina com outra linha em branco
            client.println();
            // Sair do loop while
            break;
          } else { // se você tiver uma nova linha, limpe a currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // se você tiver qualquer outra coisa que não seja um caractere de retorno de carro,
          currentLine += c;      // adicioná-lo ao final da currentLine
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