#define VERMELHO_LED 10    
#define LARANJA_LED 9
#define AMARELO_LED 8
#define AZULCLARO_LED  7 
#define VERDE_LED 6   

#define ANALOG_INPUT_PIN A0

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging

  // Declara os pinos para saida
  pinMode(VERMELHO_LED, OUTPUT);
  pinMode(LARANJA_LED, OUTPUT);
  pinMode(AMARELO_LED, OUTPUT);
  pinMode(AZULCLARO_LED, OUTPUT);
  pinMode(VERDE_LED, OUTPUT);

  // Inicializa os Led - Apagado
  digitalWrite(VERMELHO_LED, LOW);
  digitalWrite(LARANJA_LED, LOW);
  digitalWrite(AMARELO_LED, LOW);
  digitalWrite(AZULCLARO_LED, LOW);
  digitalWrite(VERDE_LED, LOW);
}

void loop() {
  // Leia os dados do sensor 
  int sensorValue = analogRead(ANALOG_INPUT_PIN);
  Serial.println(sensorValue); // Imprime o valor para teste

  if (sensorValue > 950) {
    // O valor é alto (por exemplo, muito brilhante ou próximo) -> vermelho
    digitalWrite(VERMELHO_LED, HIGH);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(VERDE_LED, LOW);
  } else if (sensorValue <= 950 and sensorValue >= 900) {
    // O valor é médio (entre 900 e 950) -> laranja
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, HIGH);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(VERDE_LED, LOW);
  } else if (sensorValue <= 901 and sensorValue >= 800) {
    // O valor é médio (entre 901 e 800) -> amarelo
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AMARELO_LED, HIGH);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(VERDE_LED, LOW);
  } else if (sensorValue <= 801 and sensorValue >= 650) {
    // O valor é médio (entre 801 e 650) -> azulclaro
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(AZULCLARO_LED, HIGH);
    digitalWrite(VERDE_LED, LOW);
  } else {
    // O valor é baixo (500 ou menos) -> verde
    digitalWrite(VERMELHO_LED, LOW);
    digitalWrite(LARANJA_LED, LOW);
    digitalWrite(AMARELO_LED, LOW);
    digitalWrite(AZULCLARO_LED, LOW);
    digitalWrite(VERDE_LED, HIGH);
  }
  
  delay(100); // Pequeno atraso para evitar mudanças muito rápidas e estabilizar leituras
}