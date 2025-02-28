  // Ligação dos pinos do TCS230 ou TCS3200 ao Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Armazena a frequência lida pelos fotodiodos
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

void setup() {
  // Configuração das saídas
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Definindo o sensorOut como uma entrada
  pinMode(sensorOut, INPUT);
  
  // Configuração da escala de frequência para 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Inicia a comunicação serial 
  Serial.begin(9600);
}
void loop() {
  // Configuração dos fotodiodos filtrados RED (R) a serem lidos
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Leitura da frequência de saída
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Impressão do valor RED (R)
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);
  
  // Configuração dos fotodiodos filtrados VERDES (G) a serem lidos
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Leitura da frequência de saída
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Impressão do valor VERDE (G)  
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);
 
  // Configuração dos fotodiodos com filtro AZUL (B) a serem lidos
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Leitura da frequência de saída
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Impressão do valor BLUE (B) 
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);
}