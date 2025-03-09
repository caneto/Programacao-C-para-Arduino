#include <RH_ASK.h>
#include <SPI.h> // Não é usado de fato, mas é necessário para compilar

RH_ASK driver;

void setup()
{
    Serial.begin(9600);	// Somente depuração
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    uint8_t buf[12];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Sem bloqueio
    {
      int i;
      // Mensagem com uma boa soma de verificação recebida, descarte-a.
      Serial.print("Message: ");
      Serial.println((char*)buf);         
    }
}