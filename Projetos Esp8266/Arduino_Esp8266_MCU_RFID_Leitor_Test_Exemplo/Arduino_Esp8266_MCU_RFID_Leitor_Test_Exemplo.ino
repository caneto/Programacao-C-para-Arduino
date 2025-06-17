#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// Saiba mais sobre o uso do SPI/I2C ou verifique a atribuição de pinos para sua placa: https://github.com/OSSLibraries/Arduino_MFRC522v2#pin-layout
MFRC522DriverPinSimple ss_pin(15);

MFRC522DriverSPI driver{ss_pin}; // Criar driver SPI
//MFRC522DriverI2C driver{};     // Criar driver I2C
MFRC522 mfrc522{driver};         // Criar instância do MFRC522

void setup() {
  Serial.begin(115200);  // Inicialização da comunicação serial
  while (!Serial);       // Não fazer nada se nenhuma porta serial estiver aberta (adicionado para Arduinos baseados no ATMEGA32U4).
  
  mfrc522.PCD_Init();    // Inicialização da placa MFRC522.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	// Mostrar detalhes do leitor de cartões PCD - MFRC522.
	Serial.println(F("Scan PICC to see UID"));
}

void loop() {
	// Reinicie o loop se nenhum cartão novo estiver presente no sensor/leitor. Isso economiza todo o processo quando ocioso.
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Selecione um dos cartões.
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  Serial.print("Card UID: ");
  MFRC522Debug::PrintUID(Serial, (mfrc522.uid));
  Serial.println();

  // Salvar o UID em uma variável String
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uidString += "0"; 
    }
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(uidString);
}