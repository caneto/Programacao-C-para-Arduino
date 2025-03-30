// Incluindo a biblioteca WiFi do ESP8266
#include <ESP8266WiFi.h>
#include "DHT.h"

// Descomente uma das linhas abaixo para o tipo de sensor DHT que você estiver usando!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Substitua por seus detalhes de rede
const char* ssid = "YOUR_NETWORK_NAME";
const char* password = "YOUR_NETWORK_PASSWORD";

// Servidor da Web na porta 80
WiFiServer server(80);

// Sensor DHT
const int DHTPin = 5;
// Inicializar o sensor DHT.
DHT dht(DHTPin, DHTTYPE);

// Variáveis temporárias
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// é executado apenas uma vez na inicialização
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();
  
  // Conexão à rede WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Iniciando o servidor da Web
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Impressão do endereço IP do ESP
  Serial.println(WiFi.localIP());
}

// é executado repetidamente
void loop() {
  // Ouvir novos clientes
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // boleano para localizar quando a solicitação http termina
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            // As leituras do sensor também podem ser “antigas” em até 2 segundos (é um sensor muito lento)
            float h = dht.readHumidity();
            // Ler a temperatura como Celsius (o padrão)
            float t = dht.readTemperature();
            // Ler a temperatura como Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);
            // Verifique se alguma leitura falhou e saia antes (para tentar novamente).
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println("Failed to read from DHT sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              // Calcula os valores de temperatura em Celsius + Fahrenheit e umidade
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // Você pode excluir os seguintes Serial.print's, pois são apenas para fins de depuração
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.print(" *F");
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.println(" *F");
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // sua página da Web real que exibe a temperatura e a umidade
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>Temperature in Celsius: ");
            client.println(celsiusTemp);
            client.println("*C</h3><h3>Temperature in Fahrenheit: ");
            client.println(fahrenheitTemp);
            client.println("*F</h3><h3>Humidity: ");
            client.println(humidityTemp);
            client.println("%</h3><h3>");
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          // quando começa a ler uma nova linha
          blank_line = true;
        }
        else if (c != '\r') {
          // quando encontra um caractere na linha atual
          blank_line = false;
        }
      }
    }  
    // fechamento da conexão com o cliente
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}   
