#include "Keypad.h"
 
const byte ROWS = 4; // numero de linhas
const byte COLS = 3; // numero de colunas
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'#','0','*'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; // pinagens de linha do teclado R1 = D8, R2 = D7, R3 = D6, R4 = D5
byte colPins[COLS] = {4, 3, 2};    // pinagem das colunas do teclado C1 = D4, C2 = D3, C3 = D2
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
  char key = keypad.getKey(); 
  if (key != NO_KEY)
    Serial.println(key);
}
