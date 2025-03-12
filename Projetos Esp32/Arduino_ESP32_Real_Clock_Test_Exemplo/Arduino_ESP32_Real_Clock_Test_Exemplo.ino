#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "RTClib.h"

RTC_DS3231 rtc;

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    4
#define OLED_CS    5
#define OLED_RESET 2
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() 
{
   Serial.begin(9600);

  if (! rtc.begin()) {
     Serial.println("Não foi possível encontrar RTC");
     while (1);
  }
 
  rtc.adjust(DateTime(__DATE__, __TIME__));
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextColor(WHITE);
  //display.startscrollright(0x00, 0x0F);
  display.setTextSize(2);
  display.setCursor(0,5);
  display.print(" Relógio ");
  display.display();
  delay(3000);
}

void loop()
{
    DateTime now = rtc.now();

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(75,0);
    display.println(now.second(), DEC);

    display.setTextSize(2);
    display.setCursor(25,0);
    display.println(":");

    display.setTextSize(2);
    display.setCursor(65,0);
    display.println(":");

    display.setTextSize(2);
    display.setCursor(40,0);
    display.println(now.minute(), DEC);

    display.setTextSize(2);
    display.setCursor(0,0);
    display.println(now.hour(), DEC);

    display.setTextSize(1);
    display.setCursor(0,15);
    display.println(now.day(), DEC);
    display.print(daysOfTheWeek[now.dayOfTheWeek()]);

    display.setTextSize(1);
    display.setCursor(25,15);
    display.println("-");

    display.setTextSize(1);
    display.setCursor(40,15);
    display.println(now.month(), DEC);

    display.setTextSize(1);
    display.setCursor(55,15);
    display.println("-");

    display.setTextSize(1);
    display.setCursor(70,15);
    display.println(now.year(), DEC);

    display.display(); 
}
