#include <SoftwareSerial.h>

// Configurar a porta serial do software
SoftwareSerial SIM900(7, 8);
// Variável para salvar os caracteres do SMS recebido
char incoming_char=0;

void setup() {
  // O Arduino se comunica com o shield GSM SIM900 a uma taxa de transmissão de 19200
  // Certifique-se de que isso corresponda à taxa de baud do seu módulo
  SIM900.begin(19200);
  // Para monitor serial
  Serial.begin(19200); 
  // Dê tempo ao seu escudo GSM para se conectar à rede
  delay(20000);

  // Comando AT para configurar o SIM900 no modo SMS
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Definir o módulo para enviar dados de SMS para a saída serial após o recebimento 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void loop() {
  // Exibir qualquer texto que o escudo GSM enviar para o monitor serial
  if(SIM900.available() >0) {
    //Obter o caractere da porta serial do celular
    incoming_char=SIM900.read(); 
    //Imprime o caractere de entrada no terminal
    Serial.print(incoming_char); 
  }
}