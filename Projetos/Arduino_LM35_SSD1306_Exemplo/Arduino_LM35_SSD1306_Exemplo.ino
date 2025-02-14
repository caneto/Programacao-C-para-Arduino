/*
 * CapSistema
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declaração para um monitor SSD1306 conectado a I2C (pinos SDA, SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int sensorPin = A0; 
float sensorValue;
float voltageOut;

float temperatureC;
float temperatureF;

// descomente se estiver usando o LM335
//float temperatureK;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 falha na alocação"));
    for(;;);
  }
  
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  voltageOut = (sensorValue * 5000) / 1024;
  
  // calcular a temperatura para o LM35 (LM35DZ)
  temperatureC = voltageOut / 10;
  temperatureF = (temperatureC * 1.8) + 32;

  // calcular a temperatura para o LM335
  //temperatureK = voltageOut / 10;
  //temperatureC = temperatureK - 273;
  //temperatureF = (temperatureC * 1.8) + 32;

  // calcular a temperatura para o LM34
  //temperatureF = voltageOut / 10;
  //temperatureC = (temperatureF - 32.0)*(5.0/9.0);

  Serial.print("Temperatura(ºC): ");
  Serial.print(temperatureC);
  Serial.print("  Temperatura(ºF): ");
  Serial.print(temperatureF);

  // limpa tela
  display.clearDisplay();

  // temperatura do visor em Celsius
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperatura: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(temperatureC);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");

  // temperatura do visor em Celsius
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Temperatura: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(temperatureF);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("F");

  display.display();    
  delay(1000);
 }