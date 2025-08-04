#define VERMELHO_LED 2    
#define AZUL_LED 3

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SSD1306_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int trigPin = 9;
const int echoPin = 10;

void setup() {
  Serial.begin (9600);  
  
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS); // Usando o endereço I2C
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  digitalWrite(VERMELHO_LED, LOW);
  digitalWrite(AZUL_LED, LOW);
  
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
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  // Como o teste é no emulador distancia min 2 cm, no real troque para '0 <'
  if (distance <= 2 || distance >= 400) { // Verifique a distância válida
      if(distance >= 400) {
        display.print("Muito Longe");
      } else { 
        display.print("Muito Proximo");
      }
      digitalWrite(VERMELHO_LED, LOW);
      digitalWrite(AZUL_LED, HIGH);
  } else {
      digitalWrite(VERMELHO_LED, HIGH);
      digitalWrite(AZUL_LED, LOW);
      display.print("Distancia:");
      display.print(distance);
      display.println(" cm");
      //Logo do Blog
      display.setCursor(0, 15);
      display.setTextSize(2);
      display.print("Blog");
      display.setCursor(0, 35);
      display.print("CapSistema");
  
  }
  display.display();
  
  // Imprimir no Monitor Serial
  
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(10); // Aguarde 1 segundo antes de medir novamente
}
