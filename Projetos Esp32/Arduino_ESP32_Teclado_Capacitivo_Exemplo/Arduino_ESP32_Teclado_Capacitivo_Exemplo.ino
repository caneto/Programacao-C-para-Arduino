#define Relay_PIN_1 15
#define Relay_PIN_2 2
#define Relay_PIN_3 4
#define Relay_PIN_4 16
// Todas as definições de pinos de relé
#define TOUCH_SENSOR_PIN_1 13
#define TOUCH_SENSOR_PIN_2 12
#define TOUCH_SENSOR_PIN_3 14
#define TOUCH_SENSOR_PIN_4 27
// todas as definições de pinos do sensor de toque
void setup() {
  Serial.begin(115200); //Comece o UART
  delay(1000); // me dê tempo para abrir o monitor serial
  pinMode(Relay_PIN_1, OUTPUT);
  pinMode(Relay_PIN_2, OUTPUT);
  pinMode(Relay_PIN_3, OUTPUT);
  pinMode(Relay_PIN_4, OUTPUT);
  // todos os pinos de relé definidos como saída
}
void loop() {
  /*
      Descomente para depuração
      Serial.println(" ");
      Serial.println("#############################");
      Serial.print("Button1: ");
      Serial.println(touchRead(TOUCH_SENSOR_PIN_1));  // get value of Touch 0 pin = GPIO 4
      Serial.print("Button2: ");
      Serial.println(touchRead(TOUCH_SENSOR_PIN_2));
      Serial.print("Button3: ");
      Serial.println(touchRead(TOUCH_SENSOR_PIN_3));
      Serial.print("Button4: ");
      Serial.println(touchRead(TOUCH_SENSOR_PIN_4));
      Serial.println("#############################");
      Serial.println(" ");
  */
  if ( touchRead(TOUCH_SENSOR_PIN_1) < 28) { // verifique se o valor chega a 28
    delay(50);
    if ( touchRead(TOUCH_SENSOR_PIN_1) < 28) { // novamente verifique se o valor chega abaixo de 28
      Serial.println("Sensor um e tocado");
      digitalWrite(Relay_PIN_1, !digitalRead(Relay_PIN_1));
    }
  } else if (touchRead(TOUCH_SENSOR_PIN_2) < 28) {
    delay(50);
    if (touchRead(TOUCH_SENSOR_PIN_2) < 28) {
      Serial.println("Sensor dois e tocado");
      digitalWrite(Relay_PIN_2, !digitalRead(Relay_PIN_2));
    }
  } else if ( touchRead(TOUCH_SENSOR_PIN_3) < 28) {
    delay(50);
    if (touchRead(TOUCH_SENSOR_PIN_3) < 28) {
      Serial.println("Sensor tres e tocado");
      digitalWrite(Relay_PIN_3, !digitalRead(Relay_PIN_3));
    }
  } else if (touchRead(TOUCH_SENSOR_PIN_4) < 28) {
    delay(50); // works as a debounce delay.
    if (touchRead(TOUCH_SENSOR_PIN_4) < 28) {
      Serial.println("Sensor quatro e tocado");
      digitalWrite(Relay_PIN_4, !digitalRead(Relay_PIN_4));
    }
  }
  delay(200);
}