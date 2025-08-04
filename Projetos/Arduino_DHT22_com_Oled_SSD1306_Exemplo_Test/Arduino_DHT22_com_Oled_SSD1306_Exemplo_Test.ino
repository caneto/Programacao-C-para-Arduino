#define LED_VERMELHO 13   

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SSD1306_I2C_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Estes são para a temperatura
#define DHTPIN 2
#define DHTTYPE DHT22
#define TIMEDHT 1000

float humidity, celsius, fahrenheit;

uint32_t timerDHT = TIMEDHT;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin (9600);  // Inicialize a comunicação serial para depuração
 
  // Inicializar tela OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Usando o endereço I2C
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  showWelcomeScreen();

  pinMode(LED_VERMELHO, OUTPUT);

  dht.begin();
  
}

void loop() {
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  getTemperature();
  displayTemperatura();

}

// Código para obter a temperatura
void getTemperature()
{
  // Aguarde um tempo entre as medições
  if ((millis() - timerDHT) > TIMEDHT) {
    // Atualize o timer
    timerDHT = millis();

    // A temperatura ou a umidade da leitura leva cerca de 250 milissegundos!
    // As leituras do sensor também podem ter até 2 segundos de 'idade' (é um sensor muito lento)
    humidity = dht.readHumidity();

    // Leia a temperatura como Celsius (o padrão)
    celsius = dht.readTemperature();

    // Leia a temperatura como Fahrenheit (isfahrenheit = true)
    fahrenheit = dht.readTemperature(true);

    // Verifique se alguma leitura falhou e saia cedo (para tentar novamente)
    if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit)) {
      Serial.println("Falha ao ler do sensor DHT!");
      return;
    }

    // Exibir no monitor serial
    Serial.print("Temperatura: ");
    Serial.print(humidity);
    Serial.print(" C | Humidade: ");
    Serial.print(celsius);
    Serial.print(" % ");
    Serial.println("");
    
  }
}

void displayTemperatura() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Medicao do Tempo");

  display.setTextSize(1);
  display.setCursor(0, 14);
  display.print("Temperatura: ");
  display.print(celsius, 1);
  display.println(" C");

  display.setCursor(0, 24);
  display.print("Humidade: ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 50);
  display.println("Blog CapSistema 2025");

  display.display();
}


// Função para exibir a mensagem de boas-vindas
void showWelcomeScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 25);
  display.println("Blog CapSistema 2025");
  display.display();
  delay(3000); // Display for 3 seconds
}
