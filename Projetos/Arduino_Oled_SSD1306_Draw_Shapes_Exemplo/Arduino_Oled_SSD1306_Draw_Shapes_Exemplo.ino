#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaração para um monitor SSD1306 conectado a I2C (pinos SDA, SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds

  // Limpar o buffer
  display.clearDisplay();

  // Desenhar um único pixel em branco
  display.drawPixel(64, 32, WHITE);
  display.display();
  delay(3000);

  // Desenhar linha
  display.clearDisplay();
  display.drawLine(0, 0, 127, 20, WHITE);
  display.display();
  delay(3000);
  
  // Desenhar retângulo
  display.clearDisplay();
  display.drawRect(30, 10, 50, 30, WHITE);
  display.display();
  delay(3000);
  // Preencher o retângulo
  display.fillRect(30, 10, 50, 30, WHITE);
  display.display();
  delay(3000);

  // Draw round rectangle
  display.clearDisplay();
  display.drawRoundRect(10, 10, 30, 50, 2, WHITE);
  display.display();
  delay(3000);
  // Preencher o retângulo redondo
  display.clearDisplay();
  display.fillRoundRect(10, 10, 30, 50, 2, WHITE);
  display.display();
  delay(3000);
  
  // Desenhar círculo
  display.clearDisplay();
  display.drawCircle(64, 32, 10, WHITE);
  display.display();
  delay(3000);
  // Preencher círculo
  display.fillCircle(64, 32, 10, WHITE);
  display.display();
  delay(3000);
  
  // Desenhar triângulo
  display.clearDisplay();
  display.drawTriangle(10, 10, 55, 20, 5, 40, WHITE);
  display.display();
  delay(3000);
  // Preencher triângulo
  display.fillTriangle(10, 10, 55, 20, 5, 40, WHITE);
  display.display();
  delay(3000);

  // Inverter e restaurar a exibição, fazendo uma pausa entre elas
  display.invertDisplay(true);
  delay(3000);
  display.invertDisplay(false);
  delay(3000);
}

void loop() {
}