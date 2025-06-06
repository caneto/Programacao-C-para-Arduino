#include<Wire.h>                   //Include Wire library for using I2C functions 
#include<SoftWire.h>    
#include <LiquidCrystal.h>         //Include LCD library for using LCD display functions 

#define MCP4725 0x60            //MCP4725 address as 0x60 Change yours accordingly

const int rs = PB11, en = PB10, d4 = PB0, d5 = PB1, d6 = PC13, d7 = PC14;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned int adc;
byte buffer[3];   

void setup() 
{
  Wire.begin();                    //Begins the I2C communication
  lcd.begin(16,2);                 //Sets LCD in 16X2 Mode
  lcd.print("CAPSISTEMA");   
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("STM32F103C8");
  lcd.setCursor(0,1);
  lcd.print("DAC with MCP4725");
  delay(2000);
  lcd.clear();
}

void loop() 
{
  buffer[0] = 0b01000000;            //Sets the buffer0 with control byte (010-Sets in Write mode)
  adc = analogRead(PA0);             //Read Analog value from pin PA0
  float ipvolt = (3.3/4096.0)* adc;  //Finding voltage formula
  buffer[1] = adc >> 4;              //Puts the most significant bit values
  buffer[2] = adc << 4;              //Puts the Least significant bit values
  unsigned int analogread = analogRead(PA1) ; //Reads analog value from PA1
  float opvolt = (3.3/4096.0)* analogread; //Finding Voltage Formula
  Wire.beginTransmission(MCP4725);         //Joins I2C bus with MCP4725 with 0x60 address
  Wire.write(buffer[0]);            //Sends the control byte to I2C 
  Wire.write(buffer[1]);            //Sends the MSB to I2C 
  Wire.write(buffer[2]);            //Sends the LSB to I2C
  Wire.endTransmission();           //Ends the transmission
  lcd.setCursor(0,0);     
  lcd.print("A IP:");
  lcd.print(adc);                   //Prints the ADC value from PA0
  lcd.setCursor(10,0);
  lcd.print("V:");                  //Prints the Input Voltage at PA0
  lcd.print(ipvolt);
  lcd.setCursor(0,1);
  lcd.print("D OP:");
  lcd.print(analogread);             //Prints the ADC value from PA1 (From DAC)
  lcd.setCursor(10,1);
  lcd.print("V:");
  lcd.print(opvolt);                 //Prints the Input Voltage at PA1 (From DAC)
  delay(500);
  lcd.clear();
}