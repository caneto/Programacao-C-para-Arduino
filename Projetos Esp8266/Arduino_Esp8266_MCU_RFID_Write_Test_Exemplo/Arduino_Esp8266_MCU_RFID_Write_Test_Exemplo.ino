#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// Saiba mais sobre o uso de SPI/I2C ou verifique a atribuição de pinos para sua placa: https://github.com/OSSLibraries/Arduino_MFRC522v2#pin-layout
MFRC522DriverPinSimple ss_pin(15);

MFRC522DriverSPI driver{ss_pin}; // Criar driver SPI
//MFRC522DriverI2C driver{};     // Criar driver I2C
MFRC522 mfrc522{driver};         // Criar instância do MFRC522

MFRC522::MIFARE_Key key;

byte blockAddress = 2;
byte newBlockData[17] = {"CapSistema - CAP"};
//byte newBlockData[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   // LIMPAR DADOS
byte bufferblocksize = 18;
byte blockDataRead[18];

void setup() {
  Serial.begin(115200);  // Inicialização da comunicação serial
  while (!Serial);       // Não fazer nada se nenhuma porta serial estiver aberta (adicionado para Arduinos baseados no ATMEGA32U4).
  
  mfrc522.PCD_Init();    // Inicialização da placa MFRC522.
  Serial.println(F("Aviso: este exemplo substitui um bloco em seu cartão, portanto, use-o com cuidado!"));
 
  // Preparar chave - todas as chaves são definidas como FFFFFFFFFFFFFF na entrega do chip pela fábrica.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  // Verificar se um novo cartão está presente
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  // UID do cartão de exibição
  Serial.print("----------------\nCard UID: ");
  MFRC522Debug::PrintUID(Serial, (mfrc522.uid));
  Serial.println();

  // Autenticar o bloco especificado usando KEY_A = 0x60
  if (mfrc522.PCD_Authenticate(0x60, blockAddress, &key, &(mfrc522.uid)) != 0) {
    Serial.println("Authentication failed.");
    return;
  }
  
  // Gravar dados no bloco especificado
  if (mfrc522.MIFARE_Write(blockAddress, newBlockData, 16) != 0) {
    Serial.println("Falha na gravação.");
  } else {
    Serial.print("Dados gravados com sucesso no bloco: ");
    Serial.println(blockAddress);
  }

  // Autenticar o bloco especificado usando KEY_A = 0x60
  if (mfrc522.PCD_Authenticate(0x60, blockAddress, &key, &(mfrc522.uid)) != 0) {
    Serial.println("Authentication failed.");
    return;
  }

  // Ler dados do bloco especificado
  if (mfrc522.MIFARE_Read(blockAddress, blockDataRead, &bufferblocksize) != 0) {
    Serial.println("Falha na leitura.");
  } else {
    Serial.println("Leia com sucesso!");
    Serial.print("Dados em bloco");
    Serial.print(blockAddress);
    Serial.print(": ");
    for (byte i = 0; i < 16; i++) {
      Serial.print((char)blockDataRead[i]);  // Imprimir como caractere
    }
    Serial.println();
  }
  
  // Interromper a comunicação com o cartão
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(2000);  // Atraso para facilitar a leitura
}