const int analogInPin = A0;  // Pino analógico do ESP8266 ADC0 = A0

int sensorValue = 0;  // valor lido do pote

void setup() {
  // inicializar a comunicação serial em 115200
  Serial.begin(115200);
}

void loop() {
  // ler o valor de entrada analógica
  sensorValue = analogRead(analogInPin);
 
  // imprimir as leituras no Monitor serial
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  
  delay(1000);
}