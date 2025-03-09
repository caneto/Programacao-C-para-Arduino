#include <SoftwareSerial.h>

// Configurar a porta serial do software
SoftwareSerial SIM900(7, 8); 
char incoming_char=0;

void setup() {
  // O Arduino se comunica com o shield GSM SIM900 a uma taxa de transmissão de 19200
  // Certifique-se de que isso corresponda à taxa de baud do seu módulo
  SIM900.begin(19200); // para shield GSM
  // Para monitor serial
  Serial.begin(19200); 
  // Dê tempo para fazer logon na rede.
  delay(20000); 
  
  SIM900.print("AT+CLIP=1\r"); // ativar a notificação de ID do chamador
  delay(100);
}

void loop() {
  // Exibir qualquer texto que o shield GSM enviar para o monitor serial
  if(SIM900.available() >0) {
    // Obter o caractere da porta serial do celular
    // Com uma chamada em andamento, uma mensagem “RING” é enviada
    incoming_char=SIM900.read();
    // Verificar se o shield está enviando uma mensagem “RING”
    if (incoming_char=='R') {
      delay(10);
      Serial.print(incoming_char);
      incoming_char=SIM900.read();
      if (incoming_char =='I') {
        delay(10);
        Serial.print(incoming_char);
        incoming_char=SIM900.read();
        if (incoming_char=='N') {
          delay(10);
          Serial.print(incoming_char);
          incoming_char=SIM900.read();
          if (incoming_char=='G') {
            delay(10);
            Serial.print(incoming_char);
            // Se a mensagem recebida do escudo for RING
            // Envie comandos ATA para atender o telefone
            SIM900.print("ATA\r");
          }
        }
      }
    }
  }
}
