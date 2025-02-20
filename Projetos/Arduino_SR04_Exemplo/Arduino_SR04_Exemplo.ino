/*
 *
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pino 11
        Echo: Echo (OUTPUT) - Pino 12
        GND: GND
 */
 
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm, inches;
 
void setup() {
  //Início da porta serial
  Serial.begin (9600);
  //Definir entradas e saídas
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  // O sensor é acionado por um pulso HIGH (alto) de 10 ou mais microssegundos.
  // Dê um pulso LOW curto antes para garantir um pulso HIGH limpo:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Ler o sinal do sensor: um pulso ALTO cuja duração é o tempo (em microssegundos) desde o envio do sinal.
  // duração é o tempo (em microssegundos) desde o envio
  // do ping até a recepção de seu eco em um objeto.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Converter o tempo em uma distância
  cm = (duration/2) / 29.1;     // Divida por 29,1 ou multiplique por 0,0343
  inches = (duration/2) / 74;   // Divida por 74 ou multiplique por 0,0135
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(250);
}