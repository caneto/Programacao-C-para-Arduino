/*  
    Arduino with PIR motion sensor
    
*/
 
int led = 13;                // o pino ao qual o LED está conectado
int sensor = 2;              // o pino ao qual o sensor está conectado
int state = LOW;             // por padrão, nenhum movimento é detectado
int val = 0;                 // variável para armazenar o status do sensor (valor)

void setup() {
  pinMode(led, OUTPUT);      // initalizar o LED como uma saída
  pinMode(sensor, INPUT);    // inicializar o sensor como uma entrada
  Serial.begin(9600);        // inicializar serial
}

void loop(){
  val = digitalRead(sensor);   // ler o valor do sensor
  if (val == HIGH) {           // Verifique se o sensor está ALTO
    digitalWrite(led, HIGH);   // Check that the sensor is HIGH
    delay(100);                // atraso de 100 milissegundos 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // atualizar o estado da variável para HIGH
    }
  } 
  else {
      digitalWrite(led, LOW); // update the variable's status to HIGH
      delay(200);             // atraso de 200 milissegundos 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // atualizar o estado da variável para LOW

    }
  }
}