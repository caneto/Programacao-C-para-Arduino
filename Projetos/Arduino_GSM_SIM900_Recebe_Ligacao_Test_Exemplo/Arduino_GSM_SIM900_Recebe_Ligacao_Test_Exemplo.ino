#include <SoftwareSerial.h>

// Configurar a porta serial do software
SoftwareSerial SIM900(7, 8); 

void setup() {
  // O Arduino se comunica com o shield GSM SIM900 a uma taxa de transmissão de 19200
  // Certifique-se de que isso corresponda à taxa de baud do seu módulo
  SIM900.begin(19200);
  // Dê tempo ao seu escudo GSM para se conectar à rede
  delay(20000);

  // Fazer a chamada telefônica
  callSomeone();
}

void loop() {
  
}

void callSomeone() {
  // SUBSTITUA OS X's PELO NÚMERO QUE VOCÊ QUER LIGAR
  // USE O CÓDIGO DE FORMATO INTERNACIONAL PARA NÚMEROS DE CELULAR
  SIM900.println("ATD + +XXXXXXXXX;");
  delay(100);
  SIM900.println();
  
  // Neste exemplo, a chamada dura apenas 30 segundos
  // Você pode editar a duração da chamada telefônica no tempo de atraso
  delay(30000);
  // Comando AT para desligar
  SIM900.println("ATH"); // hang up
}
