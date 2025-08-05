#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "youtube_tear.h" 
 
Adafruit_SSD1306 display(128, 64, &Wire, -1);
 
void setup(void) {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setTextColor(SSD1306_WHITE); 
}
 
void loop(void) {
  display.clearDisplay();
  auto frame = youtubeTearAnimation.nextFrame();
  display.drawBitmap(40, 0, frame, 48, 48, 1);
  display.setCursor(5, 46);
  display.print("Arduino e Divertido!");
  display.setCursor(5, 55);
  display.print("Blog CapSistema    ");
  display.display();
  delay(youtubeTearAnimation.delay);
}