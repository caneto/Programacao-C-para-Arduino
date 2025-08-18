#define UP 12 //Cima
#define DWN 11 //Baixo
#define LFT 10 //Esquerda
#define RHT 9 //Direita
#define MID 8 //Centro
#define SET 7 //Set
#define RST 6 //Reset

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial
  //Inicia os pinos com um resistor de pull-up ativado:
  pinMode(UP, INPUT_PULLUP);
  pinMode(DWN, INPUT_PULLUP);
  pinMode(LFT, INPUT_PULLUP);
  pinMode(RHT, INPUT_PULLUP);
  pinMode(MID, INPUT_PULLUP);
  pinMode(SET, INPUT_PULLUP);
  pinMode(RST, INPUT_PULLUP);
}

void loop() {
  lerJoystick();
}

void lerJoystick() {
  if(!digitalRead(UP))
  {
    Serial.println("Cima");
    delay(200);
  }
  if(!digitalRead(DWN))
  {
    Serial.println("Baixo");
    delay(200);
  }
  if(!digitalRead(LFT))
  {
    Serial.println("Esquerda");
    delay(200);
  }
  if(!digitalRead(RHT))
  {
    Serial.println("Direita");
    delay(200);
  }
  
  if(!digitalRead(MID))
  {
    Serial.println("Centro");
    delay(200);
  }
  if(!digitalRead(SET))
  {
    Serial.println("Set");
    delay(200);
  }
  if(!digitalRead(RST))
  {
    Serial.println("Reset");
    delay(200);
  }
}