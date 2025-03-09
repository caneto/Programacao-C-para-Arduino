#include <IRremote.h>

const int RECV_PIN = 11;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); // Inicializar o receptor
}

void loop() {
  if (IrReceiver.decode()) { // Verificar se os dados foram recebidos
    // Filtrar protocolos desconhecidos
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      IrReceiver.resume(); // Retomar a recepção do próximo sinal
      return; // Pular essa iteração do loop
    }

    // Imprimir somente dados válidos
    IrReceiver.printIRResultShort(&Serial); // Imprimir todos os dados recebidos em uma linha

    IrReceiver.resume(); // Retomar a recepção do próximo sinal
  }
  delay(100);
}