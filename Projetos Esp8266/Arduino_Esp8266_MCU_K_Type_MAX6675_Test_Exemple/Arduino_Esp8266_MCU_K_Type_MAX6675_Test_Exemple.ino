#include "max6675.h"

int thermoDO = 12;
int thermoCS = 15;
int thermoCLK = 14;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  Serial.begin(9600);

  Serial.println("MAX6675 test");
  // aguarde a estabilização do chip MAX
  delay(500);
}

void loop() {
  // teste básico de leitura, apenas imprime a temperatura atual
  
  Serial.print("C = "); 
  Serial.println(thermocouple.readCelsius());
  Serial.print("F = ");
  Serial.println(thermocouple.readFahrenheit());
 
  // basic read test, just prints the current temperature
  delay(1000);
}