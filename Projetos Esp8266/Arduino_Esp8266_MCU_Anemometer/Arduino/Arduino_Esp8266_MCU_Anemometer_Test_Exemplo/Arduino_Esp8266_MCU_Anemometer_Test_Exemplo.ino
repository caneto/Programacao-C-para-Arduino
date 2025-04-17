// Constantes (Altere as seguintes variáveis, se necessário)
const int anemometerPin = A0;  // Pino GPIO conectado ao anemômetro (pino analógico)
const float minVoltage = 0.033;  // Tensão correspondente a 0 m/s
const float maxVoltage = 3.3;  // Tensão correspondente a 32,4 m/s (velocidade máxima) (ao usar o divisor de tensão)
const float maxWindSpeed = 32.4; // Velocidade máxima do vento em m/s

// Fatores de conversão
const float mps_to_kmh = 3.6;   // 1 m/s = 3.6 km/h
const float mps_to_mph = 2.23694; // 1 m/s = 2.23694 mph

void setup() {
  Serial.begin(115200);  
}

void loop() {
  // Leia o valor analógico do anemômetro (valor ADC entre 0-1023 no ESP8266 para 0-3,3V)
  int adcValue = analogRead(anemometerPin);
  
  // Converta o valor do ADC em tensão (a faixa do ADC do ESP8266 é de 0 a 3,3 V)
  float voltage = (adcValue / 1023.00) * 3.3;
  
  // Verifique se a tensão está dentro da faixa de operação do anemômetro
  if (voltage < minVoltage) {
    voltage = minVoltage;
  } else if (voltage > maxVoltage) {
    voltage = maxVoltage;
  }
  
  // Mapear a tensão para a velocidade do vento
  float windSpeed_mps = ((voltage - minVoltage) / (maxVoltage - minVoltage)) * maxWindSpeed;

  // Converter a velocidade do vento em km/h e mph
  float windSpeed_kmh = windSpeed_mps * mps_to_kmh;
  float windSpeed_mph = windSpeed_mps * mps_to_mph;

  // Imprimir velocidade do vento
  Serial.print("Wind Speed: ");
  Serial.print(windSpeed_mps);
  Serial.print(" m/s, ");
  Serial.print(windSpeed_kmh);
  Serial.print(" km/h, ");
  Serial.print(windSpeed_mph);
  Serial.println(" mph");
  
  delay(1000); 
}