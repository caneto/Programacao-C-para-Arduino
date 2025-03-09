#include <IRremote.h>

const int RECV_PIN = 11;

const int bluePin = 10;
const int greenPin = 9;
const int yellowPin = 8;

void setup() {
  Serial.begin(9600); // Iniciar a comunicação serial
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); // Inicie o receptor

  pinMode(bluePin, OUTPUT);    // Definir os pinos como saída
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
}

void loop() {
  // Decodificar a entrada de infravermelho
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      IrReceiver.resume(); // Retomar a recepção do próximo sinal
      return; // Pular essa iteração do loop
    }

    // Imprimir o comando recebido para depuração
    IrReceiver.printIRResultShort(&Serial);

    switch (IrReceiver.decodedIRData.command) {
      case 0x01: // Comando para ligar o LED azul
        digitalWrite(bluePin, HIGH);
        Serial.println("Blue LED ON");
        break;

      case 0x02: // Comando para ligar o LED verde
        digitalWrite(greenPin, HIGH);
        Serial.println("Green LED OFF");
        break;

      case 0x03: // Comando para ligar o LED amarelo
        digitalWrite(yellowPin, HIGH);
        Serial.println("Yellow LED ON");
        break;

      case 0x04: // Comando para desligar o LED azul
        digitalWrite(bluePin, LOW);
        Serial.println("Blue LED OFF");
        break;

      case 0x05: // Comando para desligar o LED verde
        digitalWrite(greenPin, LOW);
        Serial.println("Green LED OFF");
        break;

      case 0x06: // Comando para desligar o LED amarelo
        digitalWrite(yellowPin, LOW);
        Serial.println("Yellow LED OFF");
        break;

      default: // Comando desconhecido
        Serial.println("Unknown Command");
        break;
    }

    IrReceiver.resume(); // Receber o próximo valor
  }
  delay(10);
}