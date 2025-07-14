// Declaração de conexão com LED
#define LED_PIN 2 // D4

void setup() {
  // Inicie o Serial0 para se comunicar com o computador e o Monitor Serial.
  Serial.begin(9600);
  // Inicie a saída LED.
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Se houver dados do Monitor Serial, envie e exiba-os.
  if (Serial.available()) {
    char c = Serial.read();
    Serial.write(c);
    // Se o dado for ‘0’, desligue o LED.
    if (c == '0') {
      digitalWrite(LED_PIN, HIGH);
    }
    // Se o dado for ‘0’, desligue o LED.
    if (c == '1') {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
