// nrf24_client

#include <SPI.h>
#include <RH_NRF24.h>

// Instância singleton do driver de rádio
RH_NRF24 nrf24;
// RH_NRF24 nrf24(8, 7); // usar isso para ser eletricamente compatível com o Mirf
// RH_NRF24 nrf24(8, 10);// Para o Leonardo, é necessário um pino SS explícito
// RH_NRF24 nrf24(8, 7); // Para o RFM73 no Arduino Mini

void setup() 
{
  Serial.begin(9600);
  while (!Serial); // aguarde a conexão da porta serial. Necessário apenas para o Leonardo
  if (!nrf24.init())
    Serial.println("init failed");
  // Os padrões após a inicialização são 2,402 GHz (canal 2), 2 Mbps, 0 dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");    
}


void loop()
{
  Serial.println("Sending to nrf24_server");
  // Enviar uma mensagem para nrf24_server
  uint8_t data[] = "Hello World!";
  nrf24.send(data, sizeof(data));
  
  nrf24.waitPacketSent();
  // Agora aguarde uma resposta
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (nrf24.waitAvailableTimeout(500))
  { 
    // Deve haver uma mensagem de resposta para nós agora   
    if (nrf24.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
    } else {
      Serial.println("recv failed");
    }
  } else {
    Serial.println("No reply, is nrf24_server running?");
  }
  delay(400);
}