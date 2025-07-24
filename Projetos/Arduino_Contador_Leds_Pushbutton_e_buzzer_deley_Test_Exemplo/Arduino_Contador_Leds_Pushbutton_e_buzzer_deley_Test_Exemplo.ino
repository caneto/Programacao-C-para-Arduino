const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int incButtonPin = 12;
const int decButtonPin = 0;
const int buzzerPin = 13;

int counter = 0;
bool lastIncState = LOW;
bool lastDecState = LOW;
unsigned long lastIncDebounce = 0;
unsigned long lastDecDebounce = 0;


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(incButtonPin, INPUT);
  pinMode(decButtonPin, INPUT);
  
  pinMode(buzzerPin, OUTPUT);
  
  updateTestLEDs(counter);
  updateLEDs(counter);
}

void loop() {
  handleButton(incButtonPin, lastIncState, lastIncDebounce, true);
  handleButton(decButtonPin, lastDecState, lastDecDebounce, false);
}


void handleButton(int pin, bool &lastState, unsigned long &lastTime, bool increment) {
  int reading = digitalRead(pin);

  if (reading != lastState) {
    lastTime = millis();
    Serial.print(reading);  
  }

  if (reading != lastState) { 
    delay(50);
    
    if (reading == HIGH && lastState == LOW) {
      if (increment) {
        if (counter < 10) counter++;
      } else {
        if (counter > 0) counter--;
      }

      updateLEDs(counter);
      tone(buzzerPin, 1000, 100);
    }
  }

  lastState = reading;
}

void updateLEDs(int value) {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], i < value ? HIGH : LOW);
  }
}

void updateTestLEDs(int value) {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(150);
  }
}


