// incluir bibliotecas de TFT e SPI
#include <TFT.h>  
#include <SPI.h>

// definição de pinos para o Arduino UNO
#define cs   10
#define dc   9
#define rst  8


// criar uma instância da biblioteca
TFT TFTscreen = TFT(cs, dc, rst);

void setup() {

  //inicializar a biblioteca
  TFTscreen.begin();

  // limpar a tela com um fundo preto
  TFTscreen.background(0, 0, 0);
  //definir o tamanho do texto
  TFTscreen.setTextSize(2);
}

void loop() {

  //gerar uma cor aleatória
  int redRandom = random(0, 255);
  int greenRandom = random (0, 255);
  int blueRandom = random (0, 255);
  
  // definir uma cor de fonte aleatória
  TFTscreen.stroke(redRandom, greenRandom, blueRandom);
  
  // imprimir Hello, World! no meio da tela
  TFTscreen.text("Hello, World!", 6, 57);
  
  // aguarde 200 milissegundos até mudar para a próxima cor
  delay(200);
}