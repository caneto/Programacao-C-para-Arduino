/*********
  Detalhes completos do projeto na CapSistema
  Baseado no exemplo da Biblioteca de Temperatura de Dallas
*********/

#include <OneWire.h>
#include <DallasTemperature.h>

// O fio de dados está conectado ao pino digital 4 do Arduino
#define ONE_WIRE_BUS 4

// Configurar uma instância do OneWire para se comunicar com qualquer dispositivo OneWire
OneWire oneWire(ONE_WIRE_BUS);

// Passar nossa referência oneWire para o sensor de temperatura Dallas 
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // Iniciar a comunicação serial para fins de depuração
  Serial.begin(9600);
  // Iniciar a biblioteca
  sensors.begin();
}

void loop(void){ 
  // Chame sensors.requestTemperatures() para emitir uma temperatura global e solicitações para todos os dispositivos no barramento
  sensors.requestTemperatures(); 
  
  Serial.print("Temperatura Celsius: ");
  // Por que “byIndex”? Você pode ter mais de um CI no mesmo barramento. 0 refere-se ao primeiro CI no fio
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Temperatura Fahrenheit: ");
  Serial.println(sensors.getTempFByIndex(0));
  delay(1000);
}
