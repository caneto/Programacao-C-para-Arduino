#include <AHTxx.h> 
#include <SPI.h> 
#include <Wire.h>

float temp;
float umidade;

AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR);

void setup(){
  Serial.begin(115200);
  aht20.begin();
}

void loop(){
  temp = aht20.readTemperature(); 
  umidade = aht20.readHumidity();
  delay(2000);
  Serial.println(temp);
  Serial.println(umidade);
}