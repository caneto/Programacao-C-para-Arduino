#include <RH_ASK.h>
#include <SPI.h> // Na verdade não é usado, mas é necessário para compilar

RH_ASK driver;

void setup()
{
    Serial.begin(9600);	  // Somente depuração
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    const char *msg = "Hello World!";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1000);
}