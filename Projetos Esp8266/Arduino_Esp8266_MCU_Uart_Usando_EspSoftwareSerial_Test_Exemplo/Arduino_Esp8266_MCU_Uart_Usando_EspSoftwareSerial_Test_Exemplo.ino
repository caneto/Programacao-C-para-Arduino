#include <SoftwareSerial.h>
// Declaração dos pinos RX e TX para espsoftwareserial
#define RX_PIN 4 // D2
#define TX_PIN 5 // D1

// Crie um objeto espsoftwareserial com velocidade de transmissão de 9600
SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  // Inicialize a porta serial de hardware com uma velocidade de transmissão de 115200 bauds.
  Serial.begin(115200);

  // Inicie a porta serial do software com uma velocidade de transmissão de 9600 bauds.
  mySerial.begin(9600);

  // Imprimir o aviso de início
  Serial.println("ESP8266 UART Communication Example");
}

void loop() {
  // Se houver dados da porta serial rígida, envie-os para a porta serial flexível.
  if (Serial.available()) {
    char c = Serial.read();
    mySerial.write(c);
  }

  // Se houver dados da porta serial virtual, envie-os para a porta serial física.
  if (mySerial.available()) {
    char c = mySerial.read();
    Serial.write(c);
  }
}