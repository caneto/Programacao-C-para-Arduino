// incluir bibliotecas de TFT e SPI
#include <TFT.h>  
#include <SPI.h>

// pin definition for Arduino UNO// definição de pinos para o Arduino UNO
#define cs   10
#define dc   9
#define rst  8


// create an instance of the library// criar uma instância da biblioteca
TFT TFTscreen = TFT(cs, dc, rst);

void setup() {

  //inicializar a biblioteca
  TFTscreen.begin();

  // limpar a tela com um fundo preto
  TFTscreen.background(0, 0, 0);
}

void loop() {

  //gerar uma cor aleatória
  int redRandom = random(0, 255);
  int greenRandom = random (0, 255);
  int blueRandom = random (0, 255);
  
  // definir a cor para as figuras
  TFTscreen.stroke(redRandom, greenRandom, blueRandom);

  // iluminar um único ponto
  TFTscreen.point(80,64);
  // esperar 200 milissegundos até mudar para a próxima figura
  delay(500);

  // desenhar uma linha
  TFTscreen.line(0,64,160,64);
  delay(500);

  //desenhar um quadrado
  TFTscreen.rect(50,34,60,60);
  delay(500);
    
  //desenhar um círculo
  TFTscreen.circle(80,64,30);
  delay(500);

  //apagar todos os números
  TFTscreen.background(0,0,0);
}