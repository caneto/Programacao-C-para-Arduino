#include <LedControl.h>

//  Definição dos pinos da Matriz de Led 8x8
#include <LedControl.h>
int DIN = 11;
int CS =  10;
int CLK = 9;
LedControl lc = LedControl(DIN, CLK, CS, 1);

// Definição de variáveis que acendem os Leds
byte mascaraBola = B00000100;           // Máscara que acende bola
byte mascaraPlataforma = B00000111;     // Máscara que acende plataforma do jogador
byte zero = B00000000;                  // Máscara para apagar leds

byte b1[8] = { B00000000,       // Leds todos apagados 1
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B00000000
             };

byte b2[8] = { B11111111,       // Leds todos acesos
               B11111111,
               B11111111,
               B11111111,
               B11111111,
               B11111111,
               B11111111,
               B11111111
             };

byte b3[8] = { B00000000,       // Leds todos apagados 2
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B00000000,
               B00000000
             };

// Variáveis de posição da bola
int l = 2;    // Linha atual
int c = 3;    // Coluna atual

// Variáveis de direção da bola
int v = 0;    // 0 pra baixo, 1 pra cima
int h = 0;    // 1 pra direita, 0 pra esquerda

// Pinos do Buzzer e Potenciômetro
#define buzzer 12
#define pinoPot A5

// Variáveis da plataforma do jogador
int posPlataforma = 1;  // Posição da plataforma do jogador
int potenciometro;      // Variável que recebe valor do potenciômetro

// Variáveis de Jogo
int tempoJogo = 200;    // Tempo de loop, diminui conforme passa de fase
int fase = 0;           // Fase de jogo, aumenta frequencia do som da bola
int pontos = 0;          // pontos conquistados

/*
   SETUP
*/
void setup() {

  // Configura e inicializa a Matriz de Led
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);

  Serial.begin(9600);             // Serial para debug, se necessário
  pinMode (buzzer, OUTPUT);       // Configura pino do Buzzer como saída
  randomSeed(analogRead(0));      // Deixa posição inicial da bola aleatória
  resetJogo();                    // Função para resetar variáveis do Jogo

}

/*
   LOOP
*/
void loop() {

  leituraControle();    // Faz leitura do Potenciômetro

  // Verifica posição/direção da bola versus posição da plataforma
  if ( l == 7 && ( (posPlataforma == (c + 1) && h == 0) || (posPlataforma == (c - 3) && h == 1) ) ) {     // Quicou na Quina
    quicaBolaQuina();
    printTela();
  } else {
    if ( l == 7 && ( c >= (posPlataforma + 3)  || c <= (posPlataforma - 1)  ) ) {                         // Não acertou plataforma, perdeu o jogo
      perdeuJogo();
      resetJogo();
    } else {
      moveBola();        // Quicou nas bordas ou no meio da plataforma
      printTela();
    }
  }

  // Checa quantos pontos já fez
  if (pontos >= 20) {
    tempoJogo = tempoJogo - 20;   // Se chegou a 20, diminui tempo do loop e incrementa fase
    pontos = 0;
    fase++;
  }

  delay(tempoJogo);

}

/****** FUNÇÕES *******/

void printTela() {
  b1[(l - 1)] = mascaraBola;
  b1[7] = mascaraPlataforma;
  printByte(0, b1);
}

void printByte(int lcd, byte caracter [])
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    lc.setRow(lcd, i, caracter[i]);
  }
}


void quicaBolaQuina() {
  // Move na coluna
  if (8 > c && c > 1 && h == 0) {
    mascaraBola >>= 1;
    c--;
  } else {
    if (8 > c && c > 1 && h == 1) {
      mascaraBola <<= 1;
      c++;
    } else {
      if (c == 1 && h == 1) {
        mascaraBola <<= 1;
        c++;
        h = 0;
      } else {
        if (c == 8 && h == 0) {
          mascaraBola >>= 1;
          c--;
          h = 1;
        }
      }
    }
  }

  // Move linha
  b1[6] = zero;
  l = 6;
  v = 1;
  if (h == 0) {
    h = 1;
  } else {
    h = 0;
  }

  // incrementa pontos e executa som
  pontos++;
  somBola2();
}

void leituraControle() {
  // Faz leitura analógica do potênciometro
  potenciometro = analogRead(pinoPot);

  // Verifica qual valor do potenciômetro e ajusta posição da plataforma conforme giro dele
  if (potenciometro < 300) {
    mascaraPlataforma = B00000111;
    posPlataforma = 1;
  }
  if (potenciometro >= 300 && potenciometro < 350) {
    mascaraPlataforma = B00001110;
    posPlataforma = 2;
  }
  if (potenciometro >= 350 && potenciometro < 400) {
    mascaraPlataforma = B00011100;
    posPlataforma = 3;
  }
  if (potenciometro >= 400 && potenciometro < 450) {
    mascaraPlataforma = B00111000;
    posPlataforma = 4;
  }
  if (potenciometro >= 450 && potenciometro < 500) {
    mascaraPlataforma = B01110000;
    posPlataforma = 5;
  }
  if (potenciometro >= 500) {
    mascaraPlataforma = B11100000;
    posPlataforma = 6;
  }

}

void moveBola() {

  // Move na coluna
  if (8 > c && c > 1 && h == 0) {
    mascaraBola <<= 1;
    c++;
  } else {
    if (8 > c && c > 1 && h == 1) {
      mascaraBola >>= 1;
      c--;
    } else {
      if (c == 1 && h == 1) {
        mascaraBola <<= 1;
        c++;
        h = 0;
        somBola();
      } else {
        if (c == 8 && h == 0) {
          mascaraBola >>= 1;
          c--;
          h = 1;
          somBola();
        }
      }
    }
  }

  // Move na linha
  if (7 > l && l > 1 && v == 0) {
    b1[(l - 1)] = zero;
    l++;
  } else {
    if (7 > l && l > 1 && v == 1) {
      b1[(l - 1)] = zero;
      l--;
    } else {
      if (l == 1 && v == 1) {
        b1[(l - 1)] = zero;
        l++;
        v = 0;
        somBola();
      } else {
        if (l == 7 && v == 0) {
          b1[(l - 1)] = zero;
          l--;
          v = 1;
          pontos++;
          somBola2();
        }
      }
    }
  }

}

void perdeuJogo() {
  // Coloca bola na ultiam linha
  if (h == 0) {
    mascaraBola <<= 1;
  } else {
    mascaraBola >>= 1;
  }
  mascaraBola |= mascaraPlataforma;
  b1[6] = zero;
  b1[7] = mascaraBola;
  printByte(0, b1);

  delay (4000);   // Atraso de 4 segundos

  // Faz piscar a tela e barulho diferente
  for (int i = 0; i < 10 ; i++) {
    printByte(0, b2);
    tone(buzzer, 1500);   // Som de 1.5KHz...
    delay (100);
    printByte(0, b3);
    tone(buzzer, 200);   // Som de 200Hz...
    delay (100);
  }
  noTone(buzzer);

  // Zera variáveis de Jogo
  pontos = 0;
  fase = 0;
  tempoJogo = 200;
}

void resetJogo() {

  // Ajusta todas variáveis para inicio do jogo
  c = random (2, 8); // sorteia coluna
  switch (c) {
    case 1:
      mascaraBola = B00000001;
      break;
    case 2:
      mascaraBola = B00000010;
      break;
    case 3:
      mascaraBola = B00000100;
      break;
    case 4:
      mascaraBola = B00001000;
      break;
    case 5:
      mascaraBola = B00010000;
      break;
    case 6:
      mascaraBola = B00100000;
      break;
    case 7:
      mascaraBola = B01000000;
      break;
    case 8:
      mascaraBola = B10000000;
      break;
  }

  h = 0; // sorteia direção lateral (1 pra direita, 0 pra esquerda)

  zero = B00000000; // Zerar b1
  b1[0] = zero;
  b1[1] = zero;
  b1[2] = zero;
  b1[3] = zero;
  b1[4] = zero;
  b1[5] = zero;
  b1[6] = zero;
  b1[7] = zero;

  l = 2; // fixo, sempre inicia na linha 2
  v = 0; // fixo, semrpre inicia com 0 pra baixo (1 é pra cima)

}

void somBola() {
  tone(buzzer, (850 + fase * 15));  // Som 850 Hz, fica mais agudo conforme passa de fase
  delay(20);                        // duração de 20 mseg
  tone(buzzer, (750 + fase * 15));  // Som 750 Hz, fica mais agudo conforme passa de fase
  delay(20);                        // duração de 20 mseg
  noTone(buzzer);                   // Termina som
}

void somBola2() {
  tone(buzzer, (200 + fase * 15));  // Som de 200 Hz, fica mais agudo conforme passa de fase
  delay(30);                        // duração de 30 mseg
  noTone(buzzer);                   // Termina som
}
