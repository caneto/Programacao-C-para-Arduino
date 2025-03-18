#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO ao qual o DS18B20 está conectado
const int oneWireBus = 4;     

// Configurar uma instância do OneWire para se comunicar com qualquer dispositivo OneWire
OneWire oneWire(oneWireBus);

// Passar nossa referência oneWire para o sensor de temperatura Dallas
DallasTemperature sensors(&oneWire);

void setup() {
  // Iniciar o monitor serial
  Serial.begin(115200);
  // Iniciar o sensor DS18B20
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(5000);
}