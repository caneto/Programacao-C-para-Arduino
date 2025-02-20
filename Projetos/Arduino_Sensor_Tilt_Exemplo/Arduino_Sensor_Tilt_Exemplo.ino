int ledPin = 12;         
int sensorPin = 4;
int sensorValue;
int lastTiltState = HIGH;   // a leitura anterior do sensor de inclinação
    
// as variáveis a seguir são long's porque o tempo, medido em milissegundos,
// se tornará rapidamente um número maior do que o que pode ser armazenado em um int.
long lastDebounceTime = 0;  // a última vez que o pino de saída foi alternado
long debounceDelay = 50;    // o tempo de debounce; aumente se a saída piscar
 
void setup(){
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}
 
void loop(){
  sensorValue = digitalRead(sensorPin);
  // Se o interruptor for alterado, devido a ruído ou pressão:
  if (sensorValue == lastTiltState) {
    // redefinir o cronômetro de debouncing
    lastDebounceTime = millis();
  } 
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // qualquer que seja a leitura, ela está lá há mais tempo
    // do que o atraso de debounce, portanto, considere-o como o estado atual real:
    lastTiltState = sensorValue;
  }
  digitalWrite(ledPin, lastTiltState);

  Serial.println(sensorValue);
  delay(500);
}