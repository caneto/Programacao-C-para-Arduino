#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Criar instância do MFRC522.
 
void setup() 
{
  Serial.begin(9600);   //  Iniciar uma comunicação serial
  SPI.begin();      // Iniciar o barramento SPI
  mfrc522.PCD_Init();   // Iniciar MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

}
void loop() 
{
  // Procure novos cartões
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Selecione um dos cartões
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  // Mostra o UID no monitor serial
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "BD 31 15 2B") //altere aqui o UID do(s) cartão(ões) que você deseja conceder acesso
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  } else {
    Serial.println(" Access denied");
    delay(3000);
  }
} 