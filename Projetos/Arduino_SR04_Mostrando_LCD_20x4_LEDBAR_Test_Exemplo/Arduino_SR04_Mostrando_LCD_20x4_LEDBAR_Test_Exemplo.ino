#define VERMELHO_LED 2    
#define AZUL_LED 3

#define LEDBAR_VERMELHO 8
#define LEDBAR_AMARELO 7
#define LEDBAR_BLUECLARO 6
#define LEDBAR_BLUE 5

#include <LiquidCrystal_I2C.h>

const byte I2C_ADDR = 0x27;
const byte LCD_COLUMNS = 20;
const byte LCD_LINES= 4;

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


const int trigPin = 9;
const int echoPin = 10;

void setup() {
  Serial.begin (9600);  
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Medidor de Distancia");
  
  pinMode(VERMELHO_LED, OUTPUT);
  pinMode(AZUL_LED, OUTPUT);
  pinMode(LEDBAR_VERMELHO, OUTPUT);
  pinMode(LEDBAR_AMARELO, OUTPUT);
  pinMode(LEDBAR_BLUECLARO, OUTPUT);
  pinMode(LEDBAR_BLUE, OUTPUT);

  digitalWrite(VERMELHO_LED, LOW);
  digitalWrite(AZUL_LED, LOW);
  digitalWrite(LEDBAR_VERMELHO, LOW);
  digitalWrite(LEDBAR_AMARELO, LOW);
  digitalWrite(LEDBAR_BLUECLARO, LOW);
  digitalWrite(LEDBAR_BLUE, LOW);
  
  // Configurar os pinos do sensor HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Enviar sinal do sensor HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Medir o tempo de resposta do sinal
  long duration = pulseIn(echoPin, HIGH);
  
  // Calcular a distância (cm)
  long distance = (duration * 0.0343) / 2;
  
  // Exibir a distância na tela OLED
  lcd.setCursor(0, 1);
  
  // Como o teste é no emulador distancia min 2 cm, no real troque para '0 <'
  if (distance <= 2 || distance >= 400) { // Verifique a distância válida
      if(distance >= 400) {
        lcd.setCursor(0, 1);
        lcd.print("Muito Longe        ");
        digitalWrite(LEDBAR_VERMELHO, HIGH);
        digitalWrite(LEDBAR_AMARELO, LOW);
        digitalWrite(LEDBAR_BLUECLARO, LOW);
        digitalWrite(LEDBAR_BLUE, LOW);
      } else { 
        lcd.print("Muito Proximo     ");
        digitalWrite(LEDBAR_VERMELHO, LOW);
        digitalWrite(LEDBAR_AMARELO, LOW);
        digitalWrite(LEDBAR_BLUECLARO, LOW);
        digitalWrite(LEDBAR_BLUE, HIGH);
      }
      digitalWrite(VERMELHO_LED, LOW);
      digitalWrite(AZUL_LED, HIGH);
  } else {
      digitalWrite(VERMELHO_LED, HIGH);
      digitalWrite(AZUL_LED, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Distancia:");
      lcd.print(distance);
      lcd.println(" cm");
      
      if(distance >= 300) {
        digitalWrite(LEDBAR_VERMELHO, LOW);
        digitalWrite(LEDBAR_AMARELO, HIGH);
        digitalWrite(LEDBAR_BLUECLARO, LOW);
        digitalWrite(LEDBAR_BLUE, LOW);
      } else {
        digitalWrite(LEDBAR_VERMELHO, LOW);
        digitalWrite(LEDBAR_AMARELO, LOW);
        digitalWrite(LEDBAR_BLUECLARO, HIGH);
        digitalWrite(LEDBAR_BLUE, LOW);
      }
   }
   
   //Logo do Blog
   lcd.setCursor(0, 3);
   lcd.print("Blog CapSistema 2025");
      
  // Imprimir no Monitor Serial
  
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(10); // Aguarde 1 segundo antes de medir novamente
}
