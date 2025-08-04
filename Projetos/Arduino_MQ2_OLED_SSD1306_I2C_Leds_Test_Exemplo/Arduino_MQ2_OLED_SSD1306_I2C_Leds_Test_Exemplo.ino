#define VERMELHO_LED 10    
#define LARANJA_LED 9
#define AMARELO_LED 8
#define AZULCLARO_LED  7 
#define VERDE_LED 6   

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SSD1306_I2C_ADDRESS 0x3C // Địa chỉ I2C của màn hình OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ANALOG_INPUT_PIN A0

void setup () {
  Serial.begin (9600);  // Inicialize a comunicação serial para depuração
 
  // Inicializar tela OLED
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS); // Usando o endereço I2C
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  digitalWrite(AMARELO_LED, LOW);
  digitalWrite(AZULCLARO_LED, LOW);
  digitalWrite(VERDE_LED, LOW);
}

void loop() {
  // Leia os dados do sensor 
  int sensorValue = analogRead(ANALOG_INPUT_PIN);
  Serial.println(sensorValue); // Imprime o valor para teste

  if (sensorValue > 950) {
    // O valor é alto (por exemplo, muito brilhante ou próximo) -> vermelho
    digitalWrite(VERMELHO_LED, HIGH);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(VERDE_LED, LOW);
    textLCD("Perigoso");
  } else if (sensorValue <= 950 and sensorValue >= 900) {
    // O valor é médio (entre 900 e 950) -> laranja
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, HIGH);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(VERDE_LED, LOW);
    textLCD("Ruim");
  } else if (sensorValue <= 901 and sensorValue >= 800) {
    // O valor é médio (entre 901 e 800) -> amarelo
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AMARELO_LED, HIGH);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(VERDE_LED, LOW);
    textLCD("Fraco");
  } else if (sensorValue <= 801 and sensorValue >= 650) {
    // O valor é médio (entre 801 e 650) -> azulclaro
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(AZULCLARO_LED, HIGH);
    digitalWrite(VERDE_LED, LOW);
    textLCD("Rasoavel");
  } else {
    // O valor é baixo (500 ou menos) -> verde
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(VERDE_LED, HIGH);
    textLCD("Limpo");
  }
  
  delay(100); // Pequeno atraso para evitar mudanças muito rápidas e estabilizar leituras
}

void textLCD(String text) {
  // Exibir os dados na tela OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Info Ar : ");
  display.print(text);
  display.setCursor(0, 15);
  display.setTextSize(2);
  display.print("Blog");
  display.setCursor(0, 35);
  display.print("CapSistema");
  display.display();
}
