
// O pino do relé é controlado com D8. O fio ativo é conectado ao Normalmente fechado e comum
int relay = 8;
volatile byte relayState = LOW;

// O sensor de movimento PIR está conectado a D2.
int PIRInterrupt = 2;

// Variáveis de timer
long lastDebounceTime = 0;  
long debounceDelay = 10000;

void setup() {
  // Pino para módulo de relé definido como saída
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  // Sensor de movimento PIR definido como uma entrada
  pinMode(PIRInterrupt, INPUT);
  // Aciona a função detectMotion no modo crescente para ligar o relé, se a condição for atendida
  attachInterrupt(digitalPinToInterrupt(PIRInterrupt), detectMotion, RISING);
  // Comunicação serial para fins de depuração
  Serial.begin(9600);
}

void loop() {
  // Se 10 segundos se passaram, o relé é desligado
  if((millis() - lastDebounceTime) > debounceDelay && relayState == HIGH){
    digitalWrite(relay, HIGH);
    relayState = LOW;
    Serial.println("OFF");
  }
  delay(50);
}

void detectMotion() {
  Serial.println("Motion");
  if(relayState == LOW){
    digitalWrite(relay, LOW);
  }
  relayState = HIGH;  
  Serial.println("ON");
  lastDebounceTime = millis();
}