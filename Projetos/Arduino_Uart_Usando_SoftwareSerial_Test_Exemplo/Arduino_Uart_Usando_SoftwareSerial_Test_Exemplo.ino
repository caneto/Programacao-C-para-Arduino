#include <SoftwareSerial.h> // Biblioteca para porta serial virtual
#define RX_PIN 10 // Pino RX da porta serial flexível
#define TX_PIN 11 // Pino TX da porta serial flexível
#define LED_PIN 13 // Pino conectado ao led

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Criar um objeto de porta serial

void setup() {
  Serial.begin(9600); // Inicie a porta serial com uma velocidade de transmissão de 9600 bauds.
  mySerial.begin(9600); // Inicie a porta serial com uma velocidade de transmissão de 9600 bauds.
  pinMode(LED_PIN, OUTPUT); // A conexão com o LED é a saída.
}

void loop() {
  if (mySerial.available()) { // Se houver dados da porta serial
    char c = mySerial.read(); // // Se houver dados da porta serial
    mySerial.println(c); // Imprimir caracteres de controle através da porta serial
    Serial.println(c); // Imprimir caracteres de controle através da porta serial
    if (c == '0') { // Se o caractere for '0'
      // Bật led
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED is on"); // No estado LED através da porta serial rígida
    }
    else if (c == '1') { // Se o caractere for 'f'
      // Tắt led
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED is off"); // No estado LED através da porta serial rígida
    }
    else { // Se o caractere for diferente
      // Không làm gì
      Serial.println("Invalid command"); // Relate o erro através da porta serial
    }
  }
}