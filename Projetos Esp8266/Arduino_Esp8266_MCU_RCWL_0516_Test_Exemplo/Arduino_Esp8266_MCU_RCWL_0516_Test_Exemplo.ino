// Se estiver usando o LED embutido para testes, lembre-se de que ele funciona com lógica invertida
// (HIGH=LOW e LOW=HIGH)
int led = 2;               // o pino ao qual o LED está conectado
int sensor = 12;           // o pino ao qual o sensor está conectado
int state = LOW;           // por padrão, nenhum movimento é detectado
int val = 0;               // variável para armazenar o status do sensor (valor)

void setup() {
  pinMode(led, OUTPUT);       // initalizar o LED como uma saída
  pinMode(sensor, INPUT);     // inicializar o sensor como uma entrada
  Serial.begin(115200);       // inicializar serial
}

void loop(){
  val = digitalRead(sensor);   // ler o valor do sensor
  if (val == HIGH) {           // Verifique se o sensor está ALTO
    digitalWrite(led, HIGH);   // ligar o LED
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // atualizar o estado da variável para HIGH
    }
  } 
  else {
    digitalWrite(led, LOW); // Desligar o LED
      
    if (state == HIGH){
      Serial.println("Motion stopped!");
      state = LOW;       // atualizar o estado da variável para LOW
    }
  }
}