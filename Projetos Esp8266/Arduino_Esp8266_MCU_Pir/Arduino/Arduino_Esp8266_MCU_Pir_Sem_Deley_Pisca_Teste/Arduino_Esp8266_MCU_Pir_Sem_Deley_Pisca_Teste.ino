// as constantes não serão alteradas. Usado aqui para definir um número de pino:
const int ledPin =  26;      // o número do pino do LED

// As variáveis serão alteradas:
int ledState = LOW;             // ledState usado para definir o LED

// Geralmente, você deve usar "unsigned long" para variáveis que contêm tempo
// O valor se tornará rapidamente muito grande para ser armazenado por um int
unsigned long previousMillis = 0;        // armazenará a última vez que o LED foi atualizado

// As constantes não serão alteradas:
const long interval = 1000;           // intervalo para piscar (milissegundos)

void setup() {
  // define o pino digital como saída:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // verifique se é hora de piscar o LED; isto é, se a
  // diferença entre a hora atual e a última vez que você piscou
  // LED é maior do que o intervalo no qual você deseja piscar o LED.
  // piscar o LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // salvar a última vez que você piscou o LED
    previousMillis = currentMillis;

    // Se o LED estiver desligado, ligue-o e vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // define o LED com o ledState da variável:
    digitalWrite(ledPin, ledState);
  }
}