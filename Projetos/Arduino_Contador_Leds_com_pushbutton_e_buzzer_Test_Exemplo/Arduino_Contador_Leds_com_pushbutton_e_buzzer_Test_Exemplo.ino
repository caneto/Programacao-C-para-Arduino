const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int buttonPin = 12;
const int buzzerPin = 13;

int counter = 0;
bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int reading = digitalRead(buttonPin);
   
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
    Serial.print(reading);  
  }

  if (reading != lastButtonState) { 
    delay(50);
    
    if (reading == HIGH && lastButtonState == LOW) {
      counter++;
      if (counter > 10) counter = 0;
      updateLEDs(counter);
      tone(buzzerPin, 1000, 100);  // Som de 100ms
    }
  } 

  lastButtonState = reading;
 
}

void updateLEDs(int value) {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], i < value ? HIGH : LOW);
  }
}


