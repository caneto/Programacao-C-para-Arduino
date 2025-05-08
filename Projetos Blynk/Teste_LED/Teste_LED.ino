#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL2FvPICucD"
#define BLYNK_TEMPLATE_NAME "Meu Primeiro Projeto"
#define BLYNK_AUTH_TOKEN "Re-2e2jG1-LwphzMH_npMvphLun0AoLt"

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

WidgetLED led1(V0);

BlynkTimer timer;

void blinkLedWidget()
{
  if (led1.getValue()) {
    led1.off();
    Serial.println("Desliga o LED: desligado");
  } else {
    led1.on();
    Serial.println("Liga o LED: ligado");
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);

  timer.setInterval(1000L, blinkLedWidget);
}

void loop()
{
  Blynk.run();
  timer.run();
}

