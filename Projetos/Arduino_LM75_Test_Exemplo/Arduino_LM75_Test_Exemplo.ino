#include <M2M_LM75A.h> //INCLUSÃO DE BIBLIOTECA

M2M_LM75A lm75a; //OBJETO DO TIPO M2M_LM75A (I2C)

void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  lm75a.begin(); //INICIALIZA O SENSOR
  while (!Serial); //ARDUINO LEONARDO (AGUARDA PELO MONITOR SERIAL)  
}

void loop(){
  Serial.print(F("Temperatura: ")); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(lm75a.getTemperature()); //IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
  Serial.println(F("ºC")); //IMPRIME O TEXTO NO MONITOR SERIAL
  delay(1000); //INTERVALO DE 1 SEGUNDO
}