#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Definir GPIOs para LED e reedswitch
const int reedSwitch = 4;
const int led = 2; //opcional

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

// Inicializar o Telegram BOT
#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // seu Bot Token (obtido do Botfather)

// Use @myidbot para descobrir o ID de bate-papo de um indivíduo ou de um grupo
// Observe também que você precisa clicar em "iniciar" em um bot para que ele possa
// enviar mensagens para você
#define CHAT_ID "XXXXXXXXXX"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// É executado sempre que o reedswitch muda de estado
ICACHE_RAM_ATTR void changeDoorStatus() {
  Serial.println("State changed");
  changeState = true;
}

void setup() {
  // Porta serial para fins de depuração
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");      // obter a hora UTC via NTP
  client.setTrustAnchors(&cert); // Adicionar certificado raiz para api.telegram.org
  

  // Ler o estado atual da porta
  pinMode(reedSwitch, INPUT_PULLUP);
  state = digitalRead(reedSwitch);

  // Definir o estado do LED para corresponder ao estado da porta
  pinMode(led, OUTPUT);
  digitalWrite(led, state);
  
  // Defina o pino do reedswitch como interrupção, atribua a função de interrupção e defina o modo CHANGE
  attachInterrupt(digitalPinToInterrupt(reedSwitch), changeDoorStatus, CHANGE);

  // Conectar-se ao Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  

    bot.sendMessage(CHAT_ID, "Bot started up", "");
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
        
        //Enviar notificação
        bot.sendMessage(CHAT_ID, "The door is " + doorState, "");
    }  
  }
}