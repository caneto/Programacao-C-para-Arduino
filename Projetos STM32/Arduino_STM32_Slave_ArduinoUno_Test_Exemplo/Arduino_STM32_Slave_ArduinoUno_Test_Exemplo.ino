//Slave Arduino SPI
//SPI Slave Code for Arduino
//SPI Communication between STM32F103C8 & Arduino

#include<SPI.h>                           // Including Library for using SPI Communication
#include<LiquidCrystal.h>                 // Including LCD display library

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);      // Define LCD Module Pins (RS,EN,D4,D5,D6,D7)

volatile boolean received;
volatile byte SlaveReceived,Slavesend;

void setup()
{ 
  lcd.begin(16,2);                        // Initilize LCD display
  lcd.setCursor(0,0);                     // Sets Cursor at first line of Display 
  lcd.print("CIRCUIT DIGEST");            // Prints CIRCUIT DIGEST in LCD 
  delay(3000);                            // Delay for 3 seconds
  lcd.clear();                            // Clears LCD display
  Serial.begin(9600);                     // Starts Serial Communication at Baud Rate 9600
  pinMode(MISO,OUTPUT);                   // Sets MISO as OUTPUT (Have to Send data to Master IN (STM32F103C8)
  SPCR |= _BV(SPE);                       // Turn on SPI in Slave Mode
  received = false;
  SPI.attachInterrupt();                  // Interuupt ON is set for SPI commnucation
}

ISR (SPI_STC_vect)                        // Inerrrput routine function 
{
  SlaveReceived = SPDR;                   // Value received from master STM32F103C8 is stored in variable slavereceived
  received = true;                        // Sets received as True 
}

void loop()
{ 
  int pot = analogRead(A0);               // Analog read the input pot value from analog pin A0
  Slavesend = map(pot,0,1023,0,255);      // Converts the value pot (0-1023) to (0-255) for sending to master stm32 
  SPDR = Slavesend;                       // Sends the salvesend value to master STM32F103C8 via SPDR 
  lcd.setCursor(0,0);                     
  lcd.print("Slave: Arduino");
  lcd.setCursor(0,1);                                                     
  lcd.print("MasterVal:");    
  Serial.println("Master STM32 to Slave Arduino");   
  Serial.println(SlaveReceived);                   // Puts the received value from Master STM32F103C8 at Serial Monitor                          
  lcd.print(SlaveReceived);                        // Puts the received value from Master STM32F103C8 at LCD display
  delay(500);
  lcd.clear();
}

