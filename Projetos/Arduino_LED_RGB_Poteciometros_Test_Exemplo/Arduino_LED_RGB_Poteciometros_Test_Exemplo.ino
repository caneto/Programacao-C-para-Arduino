// Pinos dos potenciômetros
const int potR = A0;
const int potG = A1;
const int potB = A2;

// Pinos dos LEDs (PWM)
const int ledR = 9;
const int ledG = 10;
const int ledB = 11;

void setup() {
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop() {
  int valR = analogRead(potR); // Lê valor 0-1023
  int valG = analogRead(potG);
  int valB = analogRead(potB);

  // Converte 0-1023 para 0-255 (PWM)
  valR = map(valR, 0, 1023, 0, 255);
  valG = map(valG, 0, 1023, 0, 255);
  valB = map(valB, 0, 1023, 0, 255);

  // Escreve valores nos pinos PWM
  analogWrite(ledR, valR);
  analogWrite(ledG, valG);
  analogWrite(ledB, valB);

  delay(10);
}
