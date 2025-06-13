#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

  // Tela OLED de inicialização no barramento número 2
  TCA9548A(2);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display.clearDisplay();

  // Tela OLED de inicialização no ônibus número 3
  TCA9548A(3);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display.clearDisplay();

  // Tela OLED de inicialização no barramento número 4
  TCA9548A(4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display.clearDisplay();

  // Init OLED display on bus number 5
  TCA9548A(5);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Limpar o buffer
  display.clearDisplay();

  // Gravação no OLED no barramento número 2
  TCA9548A(2);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Exibir texto estático
  display.println("1");
  display.display(); 

  // Gravação no OLED no barramento número 3
  TCA9548A(3);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Exibir texto estático
  display.println("2");
  display.display(); 
  
  // Gravação no OLED no barramento número 4
  TCA9548A(4);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Exibir texto estático
  display.println("3");
  display.display(); 
  
  // Gravação no OLED no barramento número 5
  TCA9548A(5);
  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  // Exibir texto estático
  display.println("4");
  display.display(); 
}
 
void loop() {
  
}