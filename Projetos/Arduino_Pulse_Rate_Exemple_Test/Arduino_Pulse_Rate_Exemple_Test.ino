// Include Library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

// 
// Configure OLED screen size in pixels
#define SCREEN_WIDTH 128 //--> OLED display width, in pixels
#define SCREEN_HEIGHT 64 //--> OLED display height, in pixels

// 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 
// Variable Declarations
unsigned long previousMillisGetHR = 0; //--> will store the last time Millis (to get Heartbeat) was updated.
unsigned long previousMillisResultHR = 0; //--> will store the last time Millis (to get BPM) was updated.

const long intervalGetHR = 20; //--> Interval for reading heart rate (Heartbeat) = 10ms.
const long intervalResultHR = 10000; //--> The reading interval for the result of the Heart Rate calculation is in 10 seconds.

int PulseSensorSignal; //--> Variable to accommodate the signal value from the sensor

const int PulseSensorHRWire = 0; //--> PulseSensor connected to ANALOG PIN 5 (A5 / ADC 5).
const int LED_D12 = 12; //--> LED to detect when the heart is beating. The LED is connected to PIN D12 on the Arduino UNO.

int UpperThreshold = 530; //--> Determine which Signal to "count as a beat", and which to ingore.
int LowerThreshold = 500; 
int cntHB = 0; //--> Variable for counting the number of heartbeats.
int Threshold = 550; 

boolean ThresholdStat = true; //--> Variable for triggers in calculating heartbeats.

int BPMval = 0; //--> Variable to hold the result of heartbeats calculation.
int x=0; //--> Variable axis x graph values to display on OLED
int y=0; //--> Variable axis y graph values to display on OLED
int lastx=0; //--> The graph's last x axis variable value to display on the OLED
int lasty=0; //--> The graph's last y axis variable value to display on the OLED

// 
// 'Heart_Icon', 16x16px
// I drew this heart icon at : http://dotmatrixtool.com/

const unsigned char Heart_Icon [] PROGMEM = {
  0x00, 0x00, 0x18, 0x30, 0x3c, 0x78, 0x7e, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xee, 0xee, 0xd5, 0x56, 
  0x7b, 0xbc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00
};

int a=0;
int lasta=0;
int lastb=0;

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

// 
//  void setup
void setup() {
  pinMode(LED_D12, OUTPUT); //--> Set LED PIN as Output.

  Serial.begin(9600); //--> Set's up Serial Communication at certain speed.

// Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseSensorHRWire);   
  pulseSensor.blinkOnPulse(LED_D12);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // Address 0x3C for 128x32 and Address 0x3D for 128x64.
  // But on my 128x64 module the 0x3D address doesn't work. What works is the 0x3C address.
  // So please try which address works on your module.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); //--> Don't proceed, loop forever
  }

  // 
  // Show initial display buffer contents on the screen
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
  // 
  // Displays BPM value reading information
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 12); //--> (x position, y position)
  display.print("     Please wait");
  display.setCursor(0, 22); //--> (x position, y position)
  display.print("     10  seconds");
  display.setCursor(0, 32); //--> (x position, y position)
  display.print("       to get");
  display.setCursor(0, 42); //--> (x position, y position)
  display.print(" the Heart Rate value");
  display.display(); 
  delay(3000);
  // 
  // Displays the initial display of BPM value
  display.clearDisplay(); //--> for Clearing the display
  display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.drawLine(0, 43, 127, 43, WHITE); //--> drawLine(x1, y1, x2, y2, color)
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 48); //--> (x position, y position)
  display.print(": 0 BPM");
  display.display(); 
  // 
  Serial.println();
  Serial.println("Please wait 10 seconds to get the BPM Value");
}
//  
//  void loop
void loop() {
  GetHeartRate(); //--> Calling the GetHeartRate() subroutine
}

//  
//  void GetHeartRate()
// This subroutine is for reading the heart rate and calculating it to get the BPM value.
// To get a BPM value based on a heart rate reading for 10 seconds.
void GetHeartRate() {
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
     int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
     PulseSensorSignal = myBPM;
                                                  // "myBPM" hold this BPM value now. 
     Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
     Serial.print("BPM: ");                        // Print phrase "BPM: " 
     Serial.println(myBPM);                        // Print the value inside of myBPM. 

     DrawGraph(); //--> Calling the DrawGraph() subroutine

     //if(a>127)
     //{
     //  display.clearDisplay();
     //  a=0;
     //  lasta=a;
     //}
 
     //ThisTime=millis();
     //int value=analogRead(0);
     //display.setTextColor(WHITE);
     int b=60-(myBPM/16);
     //display.writeLine(lasta,lastb,a,b,WHITE);
     //lastb=b;
     //lasta=a;
     
     display.fillRect(20, 48, 108, 18, BLACK);
     display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)   
     display.drawLine(0, 43, 127, 43, WHITE); //--> drawLine(x1, y1, x2, y2, color)
     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(20, 48); //--> (x position, y position)
     display.print(": ");
     display.print(myBPM);
     display.print(" BPM");
     display.display(); 
     
     //a++;

  }

  // Process of reading heart rate.
  //unsigned long currentMillisGetHR = millis();
  //if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
  //  previousMillisGetHR = currentMillisGetHR;
  //  PulseSensorSignal = analogRead(PulseSensorHRWire); //--> holds the incoming raw data. Signal value can range from 0-1024
  //  if (PulseSensorSignal > UpperThreshold && ThresholdStat == true) {
  //    cntHB++;
  //    ThresholdStat = false;
  //   digitalWrite(LED_D12,HIGH);
  //  }
  //  if (PulseSensorSignal < LowerThreshold) {
  //    ThresholdStat = true;
  //    digitalWrite(LED_D12,LOW);
  //  }
    //DrawGraph(); //--> Calling the DrawGraph() subroutine
  //}
  // 
  // The process for getting the BPM value.
  //unsigned long currentMillisResultHR = millis();
  //if (currentMillisResultHR - previousMillisResultHR >= intervalResultHR) {
  //  previousMillisResultHR = currentMillisResultHR;
  //  BPMval = cntHB * 6; //--> The taken heart rate is for 10 seconds. So to get the BPM value, the total heart rate in 10 seconds x 6.
  //  Serial.print("BPM : ");
  //  Serial.println(BPMval);
  //  display.fillRect(20, 48, 108, 18, BLACK);
  //  display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)   
  //  display.drawLine(0, 43, 127, 43, WHITE); //--> drawLine(x1, y1, x2, y2, color)
  //  display.setTextSize(2);
  //  display.setTextColor(WHITE);
  //  display.setCursor(20, 48); //--> (x position, y position)
  //  display.print(": ");
  //  display.print(BPMval);
  //  display.print(" BPM");
  //  display.display(); 
  //  cntHB = 0;
  //}
  // 
}

//  
//  Subroutines for drawing or displaying heart rate graphic signals
void DrawGraph() {
  // Condition to reset the graphic display if it fills the width of the OLED screen
  if (x > 127) {
    display.fillRect(0, 0, 128, 42, BLACK);
    x = 0;
    lastx = 0;
  }
  // 
  // Process signal data to be displayed on OLED in graphic form
  //int b=60-(PulseSensorSignal/16);
  int ySignal = PulseSensorSignal; 
  if (ySignal > 300) ySignal = 300;
  if (ySignal < 50) ySignal = 50;
  int ySignalMap = map(ySignal, 50, 300, 0, 40); //--> The y-axis used on OLEDs is from 0 to 40
  y = 40 - ySignalMap;
  // 
  // Displays the heart rate graph
  display.writeLine(lastx,lasty,x,y,WHITE);
  display.display(); 
  // 
  lastx = x;
  lasty = y;
  x++;
}