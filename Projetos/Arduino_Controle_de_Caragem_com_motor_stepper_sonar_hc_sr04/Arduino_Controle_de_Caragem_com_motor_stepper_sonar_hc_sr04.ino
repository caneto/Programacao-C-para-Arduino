#define VERDE_LED 2    
#define AZUL_LED 4

#define pino_direcao 13
#define pino_passos 12

int val;    // variable to read the value from the analog pin

const int trigPin = 9;
const int echoPin = 10;

void setup()
{
  pinMode(VERDE_LED, OUTPUT);
  pinMode(AZUL_LED, OUTPUT);

  pinMode(pino_direcao,OUTPUT);
  pinMode(pino_passos,OUTPUT);

  digitalWrite(VERDE_LED, HIGH);
  digitalWrite(AZUL_LED, LOW);
  
  // Configurar os pinos do sensor HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  // Enviar sinal do sensor HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Medir o tempo de resposta do sinal
  long duration = pulseIn(echoPin, HIGH);
  
  // Calcular a distância (cm)
  long distance = (duration * 0.0343) / 2;

  if (distance < 200 ) { // Verifique se o carro esta até 100 doa distância válida
  
     digitalWrite(pino_direcao, LOW);
     for(int x=0;x<200;x++)
     {
       digitalWrite(pino_passos, HIGH);
       delay(7);
       digitalWrite(pino_passos, LOW);
       delay(7);
     } 
     
     digitalWrite(AZUL_LED, HIGH);
     digitalWrite(VERDE_LED, LOW);
  
     // Aguarda o carro passar......
     delay(2500);
     
    digitalWrite(pino_direcao, HIGH);
    for(int x=0;x<200;x++)
    {
      digitalWrite(pino_passos, HIGH);
      delay(7);
      digitalWrite(pino_passos, LOW);
      delay(7);
    }  

    digitalWrite(AZUL_LED, LOW);
    digitalWrite(VERDE_LED, HIGH);
     
  }
   
}