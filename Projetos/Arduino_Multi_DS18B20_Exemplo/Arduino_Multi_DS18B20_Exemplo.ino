/*
 * CapSistema
*/

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

int numberOfDevices; // Número de dispositivos de temperatura encontrados

DeviceAddress tempDeviceAddress; // Usaremos essa variável para armazenar um endereço de dispositivo encontrado

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
  
  // Obter uma contagem de dispositivos no fio
  numberOfDevices = sensors.getDeviceCount();
  
  // localizar dispositivos no barramento
  Serial.print("Dispositivos de localização...");
  Serial.print("Achado ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Percorrer cada dispositivo, imprimir o endereço
  for(int i=0;i<numberOfDevices; i++) {
    // Pesquisar o endereço na rede
    if(sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Dispositivo encontrado ");
      Serial.print(i, DEC);
      Serial.print(" com endereço: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
	  Serial.print("Dispositivo fantasma encontrado em ");
	  Serial.print(i, DEC);
	  Serial.print(" mas não conseguiu detectar o endereço. Verifique a alimentação e o cabeamento");
	}
  }
}

void loop(void) { 
  sensors.requestTemperatures(); // Envie o comando para obter as temperaturas
  
  // Envie o comando para obter as temperaturas
  for(int i=0;i<numberOfDevices; i++) {
    // Procure o endereço na rede eletrônica
    if(sensors.getAddress(tempDeviceAddress, i)){
		
		// Saída da ID do dispositivo
		Serial.print("Temperatura do dispositivo: ");
		Serial.println(i,DEC);

       // Imprimir os dados
       float tempC = sensors.getTempC(tempDeviceAddress);
       Serial.print("Temp C: ");
       Serial.print(tempC);
       Serial.print(" Temp F: ");
       Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converte tempC em Fahrenheit
    } 	
  }
  delay(5000);
}

// para imprimir um endereço de dispositivo
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}