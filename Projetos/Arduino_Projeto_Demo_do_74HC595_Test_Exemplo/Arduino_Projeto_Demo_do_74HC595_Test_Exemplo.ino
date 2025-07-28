#define dadosPin 9
#define latchPin 10
#define clockPin 11

const int buzzerPin = 13;


byte leds;

void setup() {
  pinMode(dadosPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  leds = 0;

  ligar();
  tone(buzzerPin, 800, 50);
  desligar();
  tone(buzzerPin, 1000, 80);
  intercalar1();
  desligar();
  tone(buzzerPin, 1200, 100);
  intercalar2();
  tone(buzzerPin, 1400, 120);
  desligar();
  tone(buzzerPin, 400, 40);
  efeito();
  tone(buzzerPin, 1000, 40);
  desligar();
  tone(buzzerPin, 800, 80);
}

void atualizarRegistrador() {
  digitalWrite(latchPin, LOW);
  shiftOut(dadosPin, clockPin, LSBFIRST, leds );
  digitalWrite(latchPin, HIGH);
}

void ligar() {
  for (int i = 0; i < 8; i++) {
    bitSet(leds, i);
    atualizarRegistrador();
  }
  delay(1000);
}

void desligar() {
  for (int j = 8; j >= 0; j-- ) {
    bitClear(leds, j);
    atualizarRegistrador();
  }
  delay(1000);
}

void intercalar1() {
  for (int k = 0; k < 8; k++) {
    float l = k % 2;
    if (l == 0) {
      bitSet(leds, k);
      atualizarRegistrador();
    }
  }
  delay(1000);
}
void intercalar2() {
  for (int n = 0; n < 8; n++) {
    float m = n % 2;
    if (m == 1) {
      bitSet(leds, n);
      atualizarRegistrador();
    }
  }
  delay(1000);
}

void efeito() {
  atualizarRegistrador();

  for (int i = 0; i < 8; i++) {
    bitSet(leds, i);
    atualizarRegistrador();
    delay(500);
  }

  for (int j = 8; j >= 0; j-- ) {
    bitClear(leds, j);
    atualizarRegistrador();
    delay(500);
  }
  delay(1000);
}

