#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display3(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display4(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Selecione o barramento I2C
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // Endereço do TCA9548A
  Wire.write(1 << bus);          // enviar byte para selecionar o barramento
  Wire.endTransmission();
  Serial.print(bus);
}

void setup() {
  Serial.begin(115200);

  // Iniciar a comunicação I2C com o multiplexador
  Wire.begin();

  // Display OLED de inicialização no barramento número 2 (display 1)
  TCA9548A(2);
  if(!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display1.clearDisplay();

  // Tela OLED de inicialização no ônibus número 3
  TCA9548A(3);
  if(!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display2.clearDisplay();

  // Tela OLED de inicialização no barramento número 4
  TCA9548A(4);
  if(!display3.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display3.clearDisplay();

  // Tela OLED de inicialização no ônibus número 5
  TCA9548A(5);
  if(!display4.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display4.clearDisplay();

  // Gravação no OLED no barramento número 2
  TCA9548A(2);
  display1.setTextSize(8);
  display1.setTextColor(WHITE);
  display1.setCursor(45, 10);
  // Exibir texto estático
  display1.println("1");
  display1.display(); 
  
  // Gravação no OLED no barramento número 3
  TCA9548A(3);
  display2.setTextSize(8);
  display2.setTextColor(WHITE);
  display2.setCursor(45, 10);
  // Exibir texto estático
  display2.println("2");
  display2.display(); 
  
  // Gravação no OLED no barramento número 4
  TCA9548A(4);
  display3.setTextSize(8);
  display3.setTextColor(WHITE);
  display3.setCursor(45, 10);
  // Exibir texto estático
  display3.println("3");
  display3.display(); 
  
  // Gravação no OLED no barramento número 5
  TCA9548A(5);
  display4.setTextSize(8);
  display4.setTextColor(WHITE);
  display4.setCursor(45, 10);
  // Exibir texto estático
  display4.println("4");
  display4.display();
}
 
void loop() {
  
}