#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Wifi Credentials
const char* ssid = "YourSSID"; // Wifi SSID
const char* password = "YourPASS"; //Wi-FI Password
WebSocketsClient webSocket; // websocket client class instance
StaticJsonDocument<100> doc; // Allocate a static JSON document
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //SSD1306 instance
void setup() {
  // Connect to local WiFi
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print local IP address
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed")); // OLED display not found
    for (;;);
  }
  delay(2000); // wait for 2s
  display.clearDisplay(); // clear the display
  display.setTextSize(2); // set Test size to 2
  display.setTextColor(WHITE); //set display colour to "WHITE"
  //address, port, and URL path 
  webSocket.begin("192.xxx.xxx.xxx", 81, "/"); 
  // webSocket event handler
  webSocket.onEvent(webSocketEvent);
  // if connection failed retry every 5s
  webSocket.setReconnectInterval(5000);
}
void loop() {
  webSocket.loop(); // Keep the socket alive
}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  // Make sure the screen is clear
  //  u8g2.clearBuffer();
  if (type == WStype_TEXT)
  {
    DeserializationError error = deserializeJson(doc, payload); // deserialize incoming Json String
    if (error) { // Print erro msg if incomig String is not JSON formated
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    const String pin_stat = doc["PIN_Status"]; // String variable tha holds LED status
    const float t = doc["Temp"]; // Float variable that holds temperature
    const float h = doc["Hum"]; // Float variable that holds Humidity
    // Print the received data for debugging
    Serial.print(String(pin_stat));
    Serial.print(String(t));
    Serial.println(String(h));
    // Send acknowledgement
    // webSocket.sendTXT("OK");
    /* LED: OFF
       TMP: Temperature
       Hum: Humidity
    */
    display.clearDisplay(); // Clear the display
    display.setCursor(0, 0); // Set the cursor position to (0,0)
    display.println("LED:"); // Print LED: on the display
    display.setCursor(45, 0); // Set the cursor
    display.print(pin_stat); // print LED Status to the display
    display.setCursor(0, 20); // Set the cursor position (0, 20)
    display.println("TMP:"); // Print TMP: on the display
    display.setCursor(45, 20); // Set the cursor position (45, 20)
    display.print(t); // Print temperature value
    display.setCursor(0, 40); // Set the cursor position (0, 40)
    display.println("HUM:");// Print HUM: on the display
    display.setCursor(45, 40); // Set the cursor position (45, 40)
    display.print(h); // Print Humidity vsalue
    display.display(); // Show all the information on the display
  }
}