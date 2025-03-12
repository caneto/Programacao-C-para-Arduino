#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup()   
{
   Serial.begin(9600);  
   lcd.begin();  
   lcd.backlight(); 
   lcd.setCursor(0,0); 
   lcd.print("Hello, world!");
   delay(1000);
   lcd.setCursor(0,1);
   lcd.print("CapSistema");
   delay(3000);  
   lcd.clear();
   lcd.setCursor(0,0); 
   lcd.print("Use o Monitor Ser.");
   lcd.setCursor(0,1);
   lcd.print("Digite para exibir");  
}

void loop()   
{
  if (Serial.available()) {
    delay(100);
    lcd.clear();
    while (Serial.available() > 0) {
      lcd.write(Serial.read()); 
    }  
  }
}