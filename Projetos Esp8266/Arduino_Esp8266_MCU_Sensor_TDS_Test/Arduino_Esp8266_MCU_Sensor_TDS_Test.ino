#define TdsSensorPin A0
#define VREF 3.3              // tensão de referência analógica (Volt) do ADC
#define SCOUNT  30            // soma do ponto de amostragem

int analogBuffer[SCOUNT];     // armazenar o valor analógico na matriz, lido do ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float temperature = 23;       // temperatura atual para compensação

// median filtering algorithm
int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
  bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}

void setup(){
  Serial.begin(115200);
  pinMode(TdsSensorPin,INPUT);
}

void loop(){
  static unsigned long analogSampleTimepoint = millis();
  if(millis()-analogSampleTimepoint > 40U){     //A cada 40 milissegundos, leia o valor analógico do ADC
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //ler o valor analógico e armazená-lo no buffer
    analogBufferIndex++;
    if(analogBufferIndex == SCOUNT){ 
      analogBufferIndex = 0;
    }
  }   
  
  static unsigned long printTimepoint = millis();
  if(millis()-printTimepoint > 800U){
    printTimepoint = millis();
    for(copyIndex=0; copyIndex<SCOUNT; copyIndex++){
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
      
      // ler o valor analógico mais estável pelo algoritmo de filtragem mediana e converter em valor de tensão
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0;
      
      //Fórmula de compensação de temperatura: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationCoefficient = 1.0+0.02*(temperature-25.0);
      //compensação de temperatura
      float compensationVoltage=averageVoltage/compensationCoefficient;
      
      //converter valor de tensão em valor de tds
      tdsValue=(133.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 857.39*compensationVoltage)*0.5;
      
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
      Serial.print("TDS Value:");
      Serial.print(tdsValue,0);
      Serial.println("ppm");
    }
  }
}
