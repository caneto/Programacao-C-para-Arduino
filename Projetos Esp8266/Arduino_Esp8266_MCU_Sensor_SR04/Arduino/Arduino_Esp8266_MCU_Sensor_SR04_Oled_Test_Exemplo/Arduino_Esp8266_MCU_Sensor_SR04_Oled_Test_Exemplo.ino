  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  #define SCREEN_WIDTH 128 // Largura da tela OLED, em pixels
  #define SCREEN_HEIGHT 64 // Altura da tela OLED, em pixels

  // Declaração para um monitor SSD1306 conectado a I2C (pinos SDA, SCL)
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

  const int trigPin = 12;
  const int echoPin = 14;

  //definir a velocidade do som em cm/uS
  #define SOUND_SPEED 0.034
  #define CM_TO_INCH 0.393701

  long duration;
  int distanceCm;
  int distanceInch;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Define o trigPin como uma saída
  pinMode(echoPin, INPUT); // Define o echoPin como uma entrada

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(500);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
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
  distanceCm = duration * SOUND_SPEED/2;
  
  // Converter em polegadas
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Imprime a distância no Monitor serial
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  display.clearDisplay();
  display.setCursor(0, 25);
  // Display distance in cm
  display.print(distanceCm);
  display.print(" cm");
  
  // Distância de exibição em polegadas
  /*display.print(distanceInch);
  display.print(" in");*/
  display.display(); 

  delay(500);  
}