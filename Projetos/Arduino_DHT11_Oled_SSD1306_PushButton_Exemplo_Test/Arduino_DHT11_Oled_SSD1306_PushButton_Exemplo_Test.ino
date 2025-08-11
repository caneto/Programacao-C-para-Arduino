#include <Adafruit_AM2320.h>  //inclui as bibliotecas
#include "U8glib.h"
#include <DHT.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

#define pino_muda 6  //define o pino do botão
#define pino_DHT A0  //define o pino do sensor 

#define DHTTYPE DHT11 

DHT dht(pino_DHT, DHTTYPE);

int temperatura; //cria uma variavel para a temperatura
int umidade;     //cria uma variavel para a umidade
int guarda_estado = LOW; //variavel para guardar o estado do botao 
int estado = 0;

unsigned long previousMillis = 0;
//Intervalo de medicao temperatura
const long interval = 2000;

void draw()
{
  //Retangulo temperatura atual
  u8g.drawRFrame(0, 17, 127, 46, 2);
  u8g.setFont(u8g_font_fub20);
  //Atualiza a temperatura no display
  u8g.setPrintPos(40, 50);
  u8g.print(temperatura);
  //Circulo grau
  u8g.drawCircle(73, 33, 3);
  u8g.drawStr( 77, 50, "C");

  //Box superior amarelo
  u8g.setFont(u8g_font_8x13B);
  u8g.drawRBox(0, 0, 127, 16, 2);
  u8g.setColorIndex(0);
  u8g.drawStr( 21, 13, "TEMPERATURA");
  u8g.setColorIndex(1);
  
  if (guarda_estado == HIGH) //se o botao for pressionado
  {
    //retangulo umidade
    u8g.setFont(u8g_font_8x13B);
    u8g.drawRBox(0, 0, 127, 16, 2);
    u8g.setColorIndex(0);
    u8g.drawStr( 21, 13, "UMIDADE"); // escreve no topo 
    u8g.setColorIndex(1);     
  
    u8g.drawRFrame(0, 17, 127, 46, 2);
    u8g.setColorIndex(0);
    u8g.drawBox(0,17,127,46);
    u8g.setColorIndex(1);
    u8g.setFont(u8g_font_fub20);
  
    u8g.setPrintPos(40, 50);
    u8g.print(umidade); //escreve na tela a umidade em %
 
    u8g.drawStr( 77, 50, "%");
  } else //se o botao nao estiver pressionado , continua mostrando a temperatura
  {
    u8g.drawRFrame(0, 17, 127, 46, 2);
    u8g.setFont(u8g_font_fub20);
    //Atualiza a temperatura no display
    u8g.setPrintPos(40, 50);
    u8g.print(temperatura);
    //Circulo grau
    u8g.drawCircle(73, 33, 3);
    u8g.drawStr( 77, 50, "C");  
    //Box superior amarelo
    u8g.setFont(u8g_font_8x13B);
    u8g.drawRBox(0, 0, 127, 16, 2);
    u8g.setColorIndex(0);
    u8g.drawStr( 21, 13, "TEMPERATURA");
    u8g.setColorIndex(1);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(pino_muda , INPUT); //define o tipo de entrada do push button
  dht.begin();
}

void loop() {
   
  unsigned long currentMillis = millis();
  //Timer para ler o valor da temperatura e da umidade
  if (currentMillis - previousMillis >= interval)
  {
    temperatura = dht.readTemperature();
    umidade = dht.readHumidity();
    previousMillis = currentMillis;
  }

  estado = digitalRead(pino_muda);
  if (estado == HIGH) {
    guarda_estado = !guarda_estado;
    delay(500);
  }
  u8g.firstPage();
  
  // Chama a rotina de desenho na tela
  do
  {
    draw();
  } while ( u8g.nextPage() );
  
  delay(50);
}