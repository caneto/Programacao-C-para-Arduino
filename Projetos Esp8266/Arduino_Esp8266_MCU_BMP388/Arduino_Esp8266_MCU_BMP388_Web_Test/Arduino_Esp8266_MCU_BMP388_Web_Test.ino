#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

// Substitua por suas credenciais de rede
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

//Descomente se estiver usando SPI
/*#define BMP_SCK 14
#define BMP_MISO 12
#define BMP_MOSI 13
#define BMP_CS 15*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

float temp;
float pres;
float alt;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;  // Temporizador de envio de leituras

void getBMPReadings(){
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  temp = bmp.temperature;
  pres = bmp.pressure / 100.0;
  alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
}

String processor(const String& var){
  getBMPReadings();
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(temp);
  }
  else if(var == "PRESSURE"){
    return String(pres);
  }
 else if(var == "ALTITUDE"){
    return String(alt);
  }
  return "";
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>BMP388 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #0F7173; color: white; font-size: 1.4rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .reading { font-size: 2rem; }
    .card.temperature { color: #272932; }
    .card.altitude { color: #D8A47F; }
    .card.pressure { color: #F05D5E; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>BMP388 WEB SERVER</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card pressure">
        <h4><i class="fas fa-angle-double-down"></i> PRESSURE</h4><p><span class="reading"><span id="pres">%PRESSURE%</span> hPa</span></p>
      </div>
      <div class="card altitude">
        <h4><i class="fas fa-long-arrow-alt-up"></i> ALTITUDE</h4><p><span class="reading"><span id="alt">%ALTITUDE%</span> m</span></p>
      </div>
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> TEMPERATURE</h4><p><span class="reading"><span id="temp">%TEMPERATURE%</span> &deg;C</span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 source.addEventListener('temperature', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temp").innerHTML = e.data;
 }, false);
 source.addEventListener('pressure', function(e) {
  console.log("pressure", e.data);
  document.getElementById("pres").innerHTML = e.data;
 }, false);
 source.addEventListener('altitude', function(e) {
  console.log("altitude", e.data);
  document.getElementById("alt").innerHTML = e.data;
 }, false);
}
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  
  // Definir o dispositivo como uma estação Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Inicialização do sensor BMEP388
  if (!bmp.begin_I2C()) {   // modo I2C de hardware, pode passar o endereço e o fio alt
  //if (! bmp.begin_SPI(BMP_CS)) {  // modo SPI de hardware 
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // modo SPI de software
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  // Configure a sobreamostragem e a inicialização do filtro
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  //Obter leituras durante a inicialização
  getBMPReadings();

  // Manipular o servidor da Web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Manipular eventos do servidor da Web
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // enviar evento com a mensagem "hello!", id current millis
    // e definir o atraso de reconexão para 1 segundo
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    getBMPReadings();
    Serial.printf("Pressure = %.2f hPa \n", pres);
    Serial.printf("Altitude = %.2f m \n", alt);
    Serial.printf("Temperature = %.2f ºC \n", temp);
    Serial.println();

    // Enviar eventos para o servidor da Web com as leituras do sensor
    events.send("ping",NULL,millis());
    events.send(String(pres).c_str(),"pressure",millis());
    events.send(String(alt).c_str(),"altitude",millis());
    events.send(String(temp).c_str(),"temperature",millis());
    
    lastTime = millis();
  }
}