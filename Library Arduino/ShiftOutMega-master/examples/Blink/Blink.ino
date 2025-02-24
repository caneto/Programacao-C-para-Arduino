/*-----------------------------------------------------------------------
 Programa : SHIFTOUTMEGA
 Descr    : Biblioteca para controlar portas adicionais OUTPUT do Arduino 
			utilizando o CI registrador de deslocamento 74HC595
 Autor    : Fellipe Couto [ http://www.efeitonerd.com.br ]
 Data     : 17/09/2012
 -----------------------------------------------------------------------*/

/*¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨
 - Pino 8 do Arduino ligado ao pino 12 do CI.
 - Pino 11 do Arduino ligado ao pino 14 do CI.
 - Pino 12 do Arduino ligado ao pino 11 do CI.
 - Led 1 = Pino 15 do CI.
 - Led 2 = Pino 1 do CI.
 - Led 3 = Pino 2 do CI.
 - Led 4 = Pino 3 do CI.
 - Led 5 = Pino 4 do CI.
 - Led 6 = Pino 5 do CI.
 - Led 7 = Pino 6 do CI.
 - Led 8 = Pino 7 do CI.
 - Pinos 10 e 16 do CI ligados ao Vcc.
 - Pinos 8 e 13 do CI ligados ao GND.
 - Pino 9 do CI ligado ao pino 14 do próximo CI.
 ¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨*/
 
#include <ShiftOutMega.h> //Inclui a bilioteca ShiftOutMega.h

//Variáveis de uso dos registradores 74HC595
int latchPin = 8;  //Pino 8 conectado ao pino 12 do 74HC595 (Latch).
int dataPin = 11;  //Pino 11 conectado ao pino 14 do 74HC595 (Data).
int clockPin = 12; //Pino 12 conectado ao pino 11 do 74HC595 (Clock).

//Quantidade de registradores (74HC595). Para cada registrador, mais 8 saídas.
int qtdRegistradores = 1;

ShiftOutMega mega(latchPin, dataPin, clockPin, qtdRegistradores); //Inicia a biblioteca passando os parametros de uso.

void setup(){
}

void loop(){
  
  mega.shiftWrite(1, HIGH);
  mega.shiftWrite(2, HIGH);
  mega.shiftWrite(3, HIGH);
  mega.shiftWrite(4, HIGH);
  mega.shiftWrite(5, HIGH);
  mega.shiftWrite(6, HIGH);
  mega.shiftWrite(7, HIGH);
  mega.shiftWrite(8, HIGH);
  delay(500);
  
  mega.shiftWrite(1, LOW);
  mega.shiftWrite(2, LOW);
  mega.shiftWrite(3, LOW);
  mega.shiftWrite(4, LOW);
  mega.shiftWrite(5, LOW);
  mega.shiftWrite(6, LOW);
  mega.shiftWrite(7, LOW);
  mega.shiftWrite(8, LOW);
  delay(500);

}
