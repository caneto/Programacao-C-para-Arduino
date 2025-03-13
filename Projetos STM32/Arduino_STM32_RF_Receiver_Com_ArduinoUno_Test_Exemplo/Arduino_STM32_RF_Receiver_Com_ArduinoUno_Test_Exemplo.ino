//Receiver Code (Arduino UNO):
//Receiver Arduino Code
//433MHZ RF with STM32F103C8 as Transmitter
#include <RH_ASK.h>                 //Includes RadioHead Library
#include <LiquidCrystal.h>          //Includes the LCD display Library 
LiquidCrystal lcd(2,3,4,5,6,7);     //Initialize lcd with Pins connected to Arduino

String str_receive;                 //Strings to Store Value
String str_number;
String str_distance;

RH_ASK rf;                          //rf as object for RG_ASK

void setup()
{
    lcd.begin(16,2);               //Lcd set as 16x2 Mode
    lcd.print("CapSistema");       //Display Welcome message
    lcd.setCursor(0,1);
    lcd.print("RF with STM32");
    delay(5000);
    lcd.clear();
    rf.init();                     //Initialize rf Object
}

void loop()
{
    uint8_t buf[7];                       
    uint8_t buflen = sizeof(buf);
    if (rf.recv(buf, &buflen))                                  
    {
      str_receive = String((char*)buf);                          // Receive String from the Transmitter
      for (int i = 0; i < str_receive.length(); i++)             // Split string into two string
      {
        if (str_receive.substring(i, i+1) == ",")                 
        {
          str_number = str_receive.substring(0, i);
          str_distance = str_receive.substring(i+1);
          break;
        }
      }
      char numberstring[4];
      char distancestring[3];
      str_distance.toCharArray(distancestring,3);              //Convert String into Char Array
      str_number.toCharArray(numberstring,3);
      int distance = atoi(distancestring);                    //Convery Array into integer value
      int number = atoi(numberstring);
      lcd.setCursor(0,0);
      lcd.print("Number:");
      lcd.print(number);                                     //Display number value at LCD display
      lcd.setCursor(0,1);
      lcd.print("Distance :");
      lcd.print(distance);                                   //Display distance value at LCD display
      lcd.print(" cm");
      delay(1500);
      lcd.clear();
    }
}