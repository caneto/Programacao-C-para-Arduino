// incluir o código da biblioteca
#include <LiquidCrystal.h>

// inicializar a biblioteca com os números dos pinos da interface
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int potPin = A0;       // Analog pin 0 for the LED brightness potentiometer
int ledPin = 6;        // LED Digital Pin with PWM
int potValue = 0;      // variable to store the value coming from the potentiometer
int brightness = 0;    // converts the potValue into a brightness 
int pBari = 0;         // progress bar
int i = 0;             // foor loop

//caráter da barra de progresso para brilho
byte pBar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

void setup() {
  // configurar nosso led como uma OUTPUT
  pinMode(ledPin, OUTPUT); 
  // configurar o número de colunas e linhas do LCD: 
  lcd.begin(16, 2);
  // Imprima uma mensagem no LCD
  lcd.print(" LED Brightness");
  // Criar o caractere da barra de progresso
  lcd.createChar(0, pBar);
}

void loop() {
  // limpa a tela LCD
  lcd.clear();
  // Imprimir uma mensagem no LCD
  lcd.print(" LED Brightness");
  //coloque o cursor na linha número 2
  lcd.setCursor(0,1);
  // ler o valor do potenciômetro
  potValue = analogRead(potPin);        
  // transforma o potValue em um brilho para o LED
  brightness=map(potValue, 0, 1024, 0, 255); 
  // acende o LED de acordo com a luminosidade
  analogWrite(ledPin, brightness); 
  // transforma o brilho em uma porcentagem para a barra 
  pBari=map(brightness, 0, 255, 0, 17);
  // imprime a barra de progresso
  for (i=0; i < pBari; i++)
  {
    lcd.setCursor(i, 1);   
    lcd.write(byte(0));  
  }
  // atrasos de 750 ms
  delay(750);        
}