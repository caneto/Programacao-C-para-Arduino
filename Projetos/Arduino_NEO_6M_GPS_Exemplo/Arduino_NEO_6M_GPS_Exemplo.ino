#include <SoftwareSerial.h>

// A conexão serial com o módulo GPS
SoftwareSerial ss(4, 3);

void setup(){
  Serial.begin(9600);
  ss.begin(9600);
}

void loop(){
  while (ss.available() > 0){
    // obter os dados de bytes do GPS
    byte gpsData = ss.read();
    Serial.write(gpsData);
  }
}