/*
 * CapSistema
 */
 
const int sensorPin = A0; 
float sensorValue;
float voltageOut;

float temperatureC;
float temperatureF;

// descomente se estiver usando o LM335
//float temperatureK;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  voltageOut = (sensorValue * 5000) / 1024;
  
  // calcular a temperatura para o LM35 (LM35DZ)
  temperatureC = voltageOut / 10;
  temperatureF = (temperatureC * 1.8) + 32;

  // calcular a temperatura para o LM335
  //temperatureK = voltageOut / 10;
  //temperatureC = temperatureK - 273;
  //temperatureF = (temperatureC * 1.8) + 32;

  // calcular a temperatura para o LM34
  //temperatureF = voltageOut / 10;
  //temperatureC = (temperatureF - 32.0)*(5.0/9.0);

  Serial.print("Temperatura(ºC): ");
  Serial.print(temperatureC);
  Serial.print("  Temperatura(ºF): ");
  Serial.print(temperatureF);
  Serial.print("  Voltagem(mV): ");
  Serial.println(voltageOut);
  delay(1000);
}