#include <SoftwareSerial.h>

// Configurar a porta serial do software
SoftwareSerial SIM900(7, 8); 

void setup() {
  // O Arduino se comunica com o shield GSM SIM900 a uma taxa de transmissão de 19200
  // Certifique-se de que isso corresponda à taxa de baud do seu módulo
  SIM900.begin(19200);
  // Dê tempo ao seu escudo GSM para se conectar à rede
  delay(20000);   
  
  // Enviar o SMS
  sendSMS();
}

void loop() { 
  
}

void sendSMS() {
  // Comando AT para configurar o SIM900 no modo SMS
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // substitua os X's pelo número de celular do destinatário
  // USE O CÓDIGO DE FORMATO INTERNACIONAL PARA NÚMEROS DE CELULAR
  SIM900.println("AT+CMGS=\"+XXXXXXXXXXXX\""); 
  delay(100);
  
  // SUBSTITUA PELO CONTEÚDO DE SUA PRÓPRIA MENSAGEM SMS
  SIM900.println("Message example from Arduino Uno."); 
  delay(100);

  // Finalizar o comando AT com um ^Z, código ASCII 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Dê tempo ao módulo para enviar SMS
  delay(5000); 
}