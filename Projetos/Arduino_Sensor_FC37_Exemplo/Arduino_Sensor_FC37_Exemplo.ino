int rainPin = A0;
int greenLED = 6;
int redLED = 7;
// você pode ajustar o valor do limite
int thresholdValue = 500;

void setup(){
  pinMode(rainPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  Serial.begin(9600);
}

void loop() {
  // ler a entrada no pino analógico 0:
  int sensorValue = analogRead(rainPin);
  Serial.print(sensorValue);
  if(sensorValue < thresholdValue){
    Serial.println(" - It's wet");
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
  }
  else {
    Serial.println(" - It's dry");
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  }
  delay(500);
}