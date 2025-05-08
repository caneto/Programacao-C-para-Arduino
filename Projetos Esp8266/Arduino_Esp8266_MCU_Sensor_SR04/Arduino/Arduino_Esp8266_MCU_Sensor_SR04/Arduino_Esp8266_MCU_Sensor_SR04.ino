const int trigPin = 12;
const int echoPin = 14;

//definir a velocidade do som em cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Inicia a comunicação serial
  pinMode(trigPin, OUTPUT); // Define o trigPin como uma saída
  pinMode(echoPin, INPUT); // Define o echoPin como uma entrada
}

void loop() {
  // Limpa o trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Define o trigPin no estado HIGH (alto) por 10 microssegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Lê o echoPin e retorna o tempo de viagem da onda sonora em microssegundos
  duration = pulseIn(echoPin, HIGH);
  
  // Calcular a distância
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Converter em polegadas
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Imprime a distância no monitor serial
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  
  delay(1000);
}