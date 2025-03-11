#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define USERNAME "choudharyas"
#define DEVICE_ID "NodeMCU"
#define DEVICE_CREDENTIAL "FcLySVkP8YFR"
#define EMAIL_ENDPOINT "IFTTT"
#define SSID "Galaxy-M20"
#define SSID_PASSWORD "ac312124"

int Pressure, Temperature, Altitude;
Adafruit_BMP085 bmp;
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);
  thing.add_wifi(SSID, SSID_PASSWORD);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}

void loop() {
  Temperature = bmp.readTemperature();
  thing["data"] >> [](pson& out){
    out["Pressure"] = bmp.readPressure()/100;
    out["Altitude"]= bmp.readAltitude();
    out["Temperature"] = bmp.readTemperature();
     };
  thing.handle();
  thing.stream(thing["data"]);
  if(Temperature > 40){
     thing.call_endpoint( EMAIL_ENDPOINT,"data");}
  Serial.print("Sending Data");
}
