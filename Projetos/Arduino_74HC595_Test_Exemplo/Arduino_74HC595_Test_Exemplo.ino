#include <ShiftRegister74HC595.h>
#define dadosPin 9 //Pino de dados do registrador
#define latchPin 10 //Pino latch do registrador
#define clockPin 11 //Pino clock do registrador

//Objeto do tipo ShiftRegister74HC595. Parâmetros:
//<número de registradores> (pino de dados, pino de clock, pino de latch)
ShiftRegister74HC595<1> registrador(dadosPin, clockPin, latchPin); 

void desligar() { //Função para desligar todos os LEDs
  registrador.setAllLow (); //Define todos os pinos BAIXOS
  delay (1000);
}
void ligar() { //Função para ligar todos os LEDs
  registrador.setAllHigh (); //Define todos os pinos HIGH
  delay (1000);
}
void intercalar() { //Função para ligar os LEDs 1, 3, 5 e 7 e depois os LEDs 2, 4, 6 e 8
  uint8_t valorPin[] = { B10101010 }; //Aciona os LEDs 1, 3, 5 e 7
  registrador.setAll(valorPin);
  delay(1000);

  uint8_t valorPin1[] = { B01010101 }; //Aciona os LEDs 2, 4, 6 e 8
  registrador.setAll(valorPin1);
  delay(1000);
}
void efeito() { //Função para criar efeito crescente e decrescente nos LEDs
  for (int i = 0; i < 8; i++) { //Aciona todos os LEDs, um por um
    registrador.set(i, HIGH); 
    delay(500);
  }
  for (int i = 8; i > 0; i--) { //Desliga todos os LEDs, um por um
    registrador.set(i, LOW);
    delay(500);
  }
}

void setup() {
}

void loop() {
  ligar(); 
  desligar(); 
  intercalar(); 
  desligar(); 
  efeito();
  desligar();
}