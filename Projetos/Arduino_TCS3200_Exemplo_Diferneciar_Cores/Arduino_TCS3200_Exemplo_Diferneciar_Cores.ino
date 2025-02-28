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

// Armazena as cores vermelha, verde e azul
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

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
  // Remapeamento do valor da frequência RED (R) de 0 a 255
  // Você deve substituir por seus próprios valores. Aqui está um exemplo: 
  redColor = map(redFrequency, 70, 120, 255,0); // Exemplo
  //redColor = map(redFrequency, XX, XX, 255,0);
  
  // Impressão do valor RED (R)
  Serial.print("R = ");
  Serial.print(redColor);
  delay(100);
  
  // Configuração dos fotodiodos filtrados VERDES (G) a serem lidos
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Leitura da frequência de saída
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remapeamento do valor da frequência VERDE (G) de 0 a 255
  // Você deve substituir por seus próprios valores. Aqui está um exemplo: 
  greenColor = map(greenFrequency, 100, 199, 255, 0); // Exemplo
  //greenColor = map(greenFrequency, XX, XX, 255, 0);
  
  // Impressão do valor VERDE (G)  
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(100);
 
  // Configuração dos fotodiodos com filtro AZUL (B) a serem lidos
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Leitura da frequência de saída
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remapeamento do valor da frequência BLUE (B) de 0 a 255
  // Você deve substituir por seus próprios valores. Aqui está um exemplo: 
  blueColor = map(blueFrequency, 38, 84, 255, 0); // Exemplo
  //blueColor = map(blueFrequency, XX, XX, 255, 0);
  
  // Impressão do valor BLUE (B) 
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(100);

  // Verifica a cor detectada atual e imprime
  // uma mensagem no monitor serial
  if(redColor > greenColor && redColor > blueColor){
      Serial.println(" - RED detected!");
  }
  if(greenColor > redColor && greenColor > blueColor){
    Serial.println(" - GREEN detected!");
  }
  if(blueColor > redColor && blueColor > greenColor){
    Serial.println(" - BLUE detected!");
  }
}
