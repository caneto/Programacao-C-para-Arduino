int redPin = 3;     // Pino RGB vermelho -> D3
int greenPin = 5;   // Pino RGB verde -> D5
int bluePin = 6;    // Pino RGB azul -> D6

int potRed = A0;     // O potenciômetro controla o pino vermelho -> A0
int potGreen = A1;   // O potenciômetro controla o pino verde -> A1
int potBlue = A2;    // O potenciômetro controla o pino azul -> A2

void setup() {
  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  pinMode(potRed, INPUT); 
  pinMode(potGreen, INPUT); 
  pinMode(potBlue, INPUT); 
}

void loop() {
  // Lê a posição atual do potenciômetro e converte 
  // em um valor entre 0 e 255 para controlar o pino RGB correspondente com PWM
  // ÂNODO COMUM DO LED RGB
  analogWrite(redPin, 255-(255./1023.)*analogRead(potRed));
  analogWrite(greenPin, 255-(255./1023.)*analogRead(potGreen));
  analogWrite(bluePin, 255-(255./1023.)*analogRead(potBlue));
 
  // Sem comentário para CATÓIDE COMUM DE LED RGB
  /*
  analogWrite(redPin, (255./1023.)*analogRead(potRed));
  analogWrite(greenPin, (255./1023.)*analogRead(potGreen));
  analogWrite(bluePin, (255./1023.)*analogRead(potBlue));
  */
  
  delay(10);
}
