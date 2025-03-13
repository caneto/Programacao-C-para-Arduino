//Master STM32 SPI
//SPI Master code for STM32F103C8
//SPI Communication between STM32 & Arduino

#include<SPI.h>                                                               // Including Library for using SPI Communication
#define SS PA4                                                              
#include<LiquidCrystal.h>                                                     // Including LCD display library

const int rs = PB0, en = PB1, d4 = PB10 , d5 = PB11 , d6 = PC13, d7 = PC14;   // Declaring pin names and pin numbers of lcd

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);                                         // Setting lcd and its paramaters

void setup (void)
{
  lcd.begin(16,2);                                                            // Setting lcd as 16x2 mode
  lcd.setCursor(0,0);                                                         // Setting cursor at first row and first column
  lcd.print("CIRCUIT DIGEST");                                                // Puts CIRCUIT DIGEST in LCD
  delay(3000);                                                                // Delays for 3 seconds
  lcd.clear();                                                                // Clears lcd display
  Serial.begin(9600);                                                         // Starts Serial Communication at Baud Rate 9600
  pinMode(SS,OUTPUT);                                                         // Puts SS as Output
  SPI.begin();                                                                // Begins the SPI commnuication
  SPI.setClockDivider(SPI_CLOCK_DIV16);                                       // Sets clock for SPI communication at 16 (72/16=4.5Mhz)
  digitalWrite(SS,HIGH);                                                      // Setting SlaveSelect as HIGH (So master doesnt connnect with slave)
}

void loop(void)
{ 
  byte MasterSend,MasterReceive;
  int pot = analogRead(PA0);                                                   // Analog read the input pot value at pin PA0
  MasterSend = map(pot,0,4096,0,255);                                          // Used to convert pot value in terms of 0 to 255 from 0 to 4096
  digitalWrite(SS, LOW);                                                       // Starts communication with Slave connected to master
  MasterReceive=SPI.transfer(MasterSend);                                      // Send the mastersend value to slave also receives value from slave
  Serial.println("Slave Arduino to Master STM32");                             // Used in Serial Monitor
  Serial.println(MasterReceive);                                               // Puts value Received im Serail Monitor
  lcd.setCursor(0,0);                                                          
  lcd.print("Master: STM32");
  lcd.setCursor(0,1);                                                    
  lcd.print("SalveVal:");                                                         
  lcd.print(MasterReceive);                                                    // Puts the received value from slave arduino      
  delay(500);
  digitalWrite(SS, HIGH);                                                      // Again make SS line HIGH so that it doesnt communicate with Slave
  lcd.clear();
}