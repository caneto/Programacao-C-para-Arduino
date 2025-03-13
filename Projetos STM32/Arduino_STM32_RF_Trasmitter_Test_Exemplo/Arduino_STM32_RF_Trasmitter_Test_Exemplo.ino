//Transmitter code (STM32F103C8):
//433MHZ RF Trasmitter with STM32F103C8
//Transmitter Code
#include <RH_ASK.h>                             //RadioHead library
#define trigPin PB1                             //Sets the Trigpin of Ultrasonic sensor as PB1
#define echoPin PB0                             //Sets the echoPin of Ultrasonic sensor as PB0

RH_ASK rf_driver(2000, PA9, PA10);              //Sets Pin PA9 as receiver and PA10 as transmitterand 2000 as Speed

String transmit_pwm;                            //Strings to store string value
String transmit_distance;
String transmit;

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    Serial.begin(9600);
    pinMode(PA0,INPUT);
    pinMode(echoPin,INPUT);
    pinMode(trigPin,OUTPUT);
}

void loop()
{
    int analoginput = analogRead(PA0);                    // ADC value from pin PA0 connected to Potentiometer
    int pwmvalue = map(analoginput,0,4096,0,100);         // Converts 0 to 4096 into 0 to 100
    digitalWrite(trigPin, LOW);                           //Makes TrigPin of Ultrasonic LOW
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);                          //Makes TrigPin of Ultrasonic HIGH
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);                           //Makes TrigPin of Ultrasonic LOW
    long duration = pulseIn(echoPin, HIGH);               //Receives the Echo signal reflected
    float distance= duration*0.034/2;                     //Calculates distance in CM of object
    transmit_pwm = String(pwmvalue);                      //Convert value into string
    transmit_distance = String(distance);                 //Convert value into string
    transmit = transmit_pwm + "," + transmit_distance;    //Adds two String in one line
    const char *msg = transmit.c_str();                   //
    rf_driver.send((uint8_t *)msg, strlen(msg));          //Sends the String
    rf_driver.waitPacketSent();         
    Serial.println(msg);                                  //Serial Print value for debug
    delay(1000);
}