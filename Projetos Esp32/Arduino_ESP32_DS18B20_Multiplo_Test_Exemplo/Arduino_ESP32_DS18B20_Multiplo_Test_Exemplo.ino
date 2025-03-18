#include <OneWire.h>
#include <DallasTemperature.h>

// O fio de dados está conectado ao GPIO 4
#define ONE_WIRE_BUS 4

// Configure uma instância do OneWire para se comunicar com qualquer dispositivo OneWire (não apenas com os CIs de temperatura Maxim/Dallas)
OneWire oneWire(ONE_WIRE_BUS);

// Passar nossa referência oneWire para o Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Número de dispositivos de temperatura encontrados
int numberOfDevices;

// Usaremos essa variável para armazenar um endereço de dispositivo encontrado
DeviceAddress tempDeviceAddress; 

void setup(){
  // iniciar a porta serial
  Serial.begin(115200);
  
  // Iniciar a biblioteca
  sensors.begin();
  
  // Obter uma contagem de dispositivos no fio
  numberOfDevices = sensors.getDeviceCount();
  
  // localizar dispositivos no barramento
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Percorrer cada dispositivo, imprimir o endereço
  for(int i=0;i<numberOfDevices; i++){
    // Pesquisar o endereço na rede
    if(sensors.getAddress(tempDeviceAddress, i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}

void loop(){ 
  sensors.requestTemperatures(); // Enviar o comando para obter temperaturas
  
  // Percorrer cada dispositivo, imprimir os dados de temperatura
  for(int i=0;i<numberOfDevices; i++){
    // Pesquisar o endereço na rede
    if(sensors.getAddress(tempDeviceAddress, i)){
      // Emite a ID do dispositivo
      Serial.print("Temperature for device: ");
      Serial.println(i,DEC);
      // Imprimir os dados
      float tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print("Temp C: ");
      Serial.print(tempC);
      Serial.print(" Temp F: ");
      Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
    }
  }
  delay(5000);
}

// função para imprimir um endereço de dispositivo
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}