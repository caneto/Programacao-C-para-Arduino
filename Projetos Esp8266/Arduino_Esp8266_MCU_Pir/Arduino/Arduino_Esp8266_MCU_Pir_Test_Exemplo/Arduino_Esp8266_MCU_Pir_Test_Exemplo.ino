#define timeSeconds 10

// Definir GPIOs para LED e sensor de movimento PIR
const int led = 12;
const int motionSensor = 14;

// Temporizador: variáveis auxiliares
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

// Verifica se o movimento foi detectado, define o LED HIGH e inicia um cronômetro
ICACHE_RAM_ATTR void detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  // Porta serial para fins de depuração
  Serial.begin(115200);
  
  // Modo do sensor de movimento PIR INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Defina o pino motionSensor como interrupção, atribua a função de interrupção e defina o modo RISING
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Ajuste o LED para LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  // Hora atual
  now = millis();
  // Desligue o LED após o número de segundos definido na variável timeSeconds
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    startTimer = false;
  }
}