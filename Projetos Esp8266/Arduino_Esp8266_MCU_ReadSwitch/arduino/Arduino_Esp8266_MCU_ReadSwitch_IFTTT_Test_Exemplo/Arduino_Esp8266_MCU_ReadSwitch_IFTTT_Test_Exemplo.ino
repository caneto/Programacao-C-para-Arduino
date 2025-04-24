#include <ESP8266WiFi.h>

// Definir GPIOs para LED e reedswitch
const int reedSwitch = 4;
const int led = 2; //optional

// Detecta sempre que a porta muda de estado
bool changeState = false;

// Mantém o estado do reedswitch (1=aberto, 0=fechado)
bool state;
String doorState;

// Variáveis auxiliares (somente detectará alterações com intervalo de 1500 milissegundos)
unsigned long previousMillis = 0; 
const long interval = 1500;

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
const char* host = "maker.ifttt.com";
const char* apiKey = "REPLACE_WITH_YOUR_IFTTT_API_KEY";

// É executado sempre que o reedswitch muda de estado
ICACHE_RAM_ATTR void changeDoorStatus() {
  Serial.println("State changed");
  changeState = true;
}

void setup() {
  // Porta serial para fins de depuraçãos
  Serial.begin(115200);

  // Ler o estado atual da porta
  pinMode(reedSwitch, INPUT_PULLUP);
  state = digitalRead(reedSwitch);

  // Definir o estado do LED para corresponder ao estado da porta
  pinMode(led, OUTPUT);
  digitalWrite(led, state);
  
  // Defina o pino do reedswitch como interrupção, atribua a função de interrupção e defina o modo CHANGE
  attachInterrupt(digitalPinToInterrupt(reedSwitch), changeDoorStatus, CHANGE);

  // Conectar-se ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
}

void loop() {
  if (changeState){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // Se um estado tiver ocorrido, inverta o estado atual da porta  
      state = !state;
      if(state) {
        doorState = "closed";
      }
      else{
        doorState = "open";
      }
      digitalWrite(led, state);
      changeState = false;
      Serial.println(state);
      Serial.println(doorState);
        
      //Enviar e-mail
      Serial.print("connecting to ");
      Serial.println(host);
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
    
      String url = "/trigger/door_status/with/key/";
      url += apiKey;
          
      Serial.print("Requesting URL: ");
      Serial.println(url);
      client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                          "Host: " + host + "\r\n" + 
                          "Content-Type: application/x-www-form-urlencoded\r\n" + 
                          "Content-Length: 13\r\n\r\n" +
                          "value1=" + doorState + "\r\n");
    }  
  }
}