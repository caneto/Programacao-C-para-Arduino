int velFrame = 25; //QUANTIDADE DE VEZES QUE O FRA SERÁ REPETIDO
int pauseDelay = 500;  //INTERVALO DE 500 MILISSEGUNDOS ENTRE AS LINHAS

char msgExibida[] = " CAPSISTEMA ";  //MENSAGEM QUE SERÁ EXIBIDA PELA MATRIZ

//Variáveis usadas para rolagem ambas começam em 0
int index = 0;  //CARACTERE CORRENTE NA STRING QUE ESTÁ SENDO MOSTRADA
int offset = 0; //QUANTIDADE DE COLUNAS DO OFFSET

int rowA[] = {13,8,17,10,5,16,4,14}; //VARIÁVEL RECEBE A PINAGEM A SER UTILIZADA PARA CONTROLE DAS LINAHS DA MATRIZ
//AS LINHAS SÃO ANODO COMUM

int colA[] = {9,3,2,12,15,11,7,6}; //VARIÁVEL RECEBE A PINAGEM A SER UTILIZADA PARA CONTROLE DAS COLUNAS DA MATRIZ
//AS COLUNAS SÃO CATODO COMUM

//CONSTANTES DEFININDO CADA POSIÇÃO DE CARACTERE NUMA MATRIZ DE INTEIROS (LETRAS)
const int A = 0;  const int B = 1;  const int C = 2;  const int D = 3;  const int E = 4;
const int F = 5;  const int G = 6;  const int H = 7;  const int I = 8;  const int J = 9;  
const int K = 10; const int L =11;  const int M = 12; const int N = 13; const int O = 14; 
const int P = 15; const int Q =16;  const int R = 17; const int S = 18; const int T = 19; 
const int U = 20; const int V =21;  const int W = 22; const int X = 23; const int Y = 24; 
const int Z = 25;

//PONTUAÇÃO
const int COL =26; const int DASH = 27; const int BRA2 = 28; const int  _ = 29; const int LINE = 34;
const int DOT =36;

//CARACTERES EXTRAS
const int  FULL =30; const int CHECK = 31; const int B2 = 32; const int TEMP = 33; 
const int SMILE =35; const int COLDOT = 36;

//MATRIZ USADA PARA ARMAZENAR UM MAPA DE BITS A SER MOSTRADO / PODE SER ALTERADO CASO QUEIRA USAR OUTRO MAPA
byte data[] = {0,0,0,0,0,0,0,0};        

//CADA CARACTERE É UM MAPA DE BITS DE 8X7 ONDE 1 É LIGADO E 0 DESLIGADO
const int _A[] = {B0001000,
                  B0010100,
                  B0100010,
                  B1000001,
                  B1111111,
                  B1000001,
                  B1000001,
                  B0000000};

const int _B[] = {B1111110,
                  B0100001,
                  B0100001,
                  B0111110,
                  B0100001,
                  B0100001,
                  B1111110,
                  B0000000};

const int _C[] = {B0011111,
                  B0100000,
                  B1000000,
                  B1000000,
                  B1000000,
                  B0100000,
                  B0011111,
                  B0000000};

const int _D[] = {B1111100,
                  B0100010,
                  B0100001,
                  B0100001,
                  B0100001,
                  B0100010,
                  B1111100,
                  B0000000};

const int _E[] = {B1111111,
                  B1000000,
                  B1000000,
                  B1111100,
                  B1000000,
                  B1000000,
                  B1111111,
                  B0000000};

const int _F[] = {B1111111,
                  B1000000,
                  B1000000,
                  B1111100,
                  B1000000,
                  B1000000,
                  B1000000,
                  B0000000};

const int _G[] = {B0011111,
                  B0100000,
                  B1000000,
                  B1001111,
                  B1000001,
                  B0100001,
                  B0011111,
                  B0000000};

const int _H[] = {B1000001,
                  B1000001,
                  B1000001,
                  B1111111,
                  B1000001,
                  B1000001,
                  B1000001,
                  B0000000};

const int _I[] = {B1111111,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0001000,
                  B1111111,
                  B0000000};

const int _J[] = {B0001111,
                  B0000001,
                  B0000001,
                  B0000001,
                  B0000001,
                  B1000001,
                  B0111110,
                  B0000000};

const int _K[] = {B1000011,
                  B1000100,
                  B1001000,
                  B1110000,
                  B1001000,
                  B1000100,
                  B1000011,
                  B0000000};

const int _L[] = {B1000000,
                  B1000000,
                  B1000000,
                  B1000000,
                  B1000000,
                  B1000000,
                  B1111111,
                  B0000000};

const int _M[] = {B1110110,
                  B1001001,
                  B1001001,
                  B1001001,
                  B1001001,
                  B1001001,
                  B1001001,
                  B0000000};

const int _N[] = {B1000001,
                  B1100001,
                  B1010001,
                  B1001001,
                  B1000101,
                  B1000011,
                  B1000001,
                  B0000000};

const int _O[] = {B0011100,
                  B0100010,
                  B1000001,
                  B1001001,
                  B1000001,
                  B0100010,
                  B0011100,
                  B0000000};

const int _P[] = {B1111110,
                  B0100001,
                  B0100001,
                  B0111110,
                  B0100000,
                  B0100000,
                  B0100000,
                  B0000000};

const int _Q[] = {B0011100,
                  B0100010,
                  B1000001,
                  B1000001,
                  B1000101,
                  B0100010,
                  B0011101,
                  B0000000};

const int _R[] = {B1111110,
                  B0100001,
                  B0100001,
                  B0101110,
                  B0100100,
                  B0100010,
                  B0100001,
                  B0000000};

const int _S[] = {B0111111,
                  B1000000,
                  B1000000,
                  B0111110,
                  B0000001,
                  B0000001,
                  B1111110,
                  B0000000};

const int _T[] = {B1111111,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0000000};

const int _U[] = {B1000001,
                  B1000001,
                  B1000001,
                  B1000001,
                  B1000001,
                  B1000001,
                  B0111110,
                  B0000000};

const int _V[] = {B1000001,
                  B1000001,
                  B1000001,
                  B1000001,
                  B0100010,
                  B0010100,
                  B0001000,
                  B0000000};
                  
const int _W[] = {B1000001,
                  B1001001,
                  B1001001,
                  B1001001,
                  B1001001,
                  B1001001,
                  B0110110,
                  B0000000};

const int _X[] = {B1000001,
                  B0100010,
                  B0010100,
                  B0001000,
                  B0010100,
                  B0100010,
                  B1000001,
                  B0000000};

const int _Y[] = {B1000001,
                  B0100010,
                  B0010100,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0001000,
                  B0000000};

const int _Z[] = {B1111111,
                  B0000010,
                  B0000100,
                  B0111110,
                  B0010000,
                  B0100000,
                  B1111111,
                  B0000000};

const int _COL[] = {B0000000,
                    B0011000,
                    B0011000,
                    B0000000,
                    B0011000,
                    B0011000,
                    B0000000,
                    B0000000};

const int _DASH[] = {B0000000,
                     B0000000,
                     B0000000,
                     B0111110,
                     B0000000,
                     B0000000,
                     B0000000,
                     B0000000};

const int _BRA2[] = {B0010000,
                     B0001000,
                     B0000100,
                     B0000100,
                     B0001000,
                     B0010000,
                     B0000000,
                     B0000000};                  

const int __[] = {B0000000,
                  B0000000,
                  B0000000,
                  B0000000,
                  B0000000,
                  B0000000,
                  B0000000,
                  B0000000};

const int _FULL[] = {B1111111,
                     B1111111,
                     B1111111,
                     B1111111,
                     B1111111,
                     B1111111,
                     B1111111,
                     B0000000};                  

const int _CHECK[] = {B1010101,
                      B0101010,
                      B1010101,
                      B0101010,
                      B1010101,
                      B0101010,
                      B1010101,
                      B0000000};
                  
const int _B2[] = {B0111110,
                   B0000001,
                   B0000001,
                   B0001111,
                   B0000001,
                   B1000001,
                   B0111110,
                   B0000000};

const int _TEMP[] = {B0000011,
                     B0011111,
                     B0111111,
                     B1111110,
                     B1111111,
                     B0011111,
                     B0000011,
                     B0000000};

const int _LINE[] = {B0000001,
                     B0000001,
                     B0000001,
                     B0000001,
                     B0000001,
                     B0000001,
                     B0000001,
                     B0000000};                     
                 
const int _SMILE[] = {B000000,
                      B1100100,
                      B1100010,
                      B0011001,
                      B1100010,
                      B1100100,
                      B0000000,
                      B0000000};                     
                  

const int _DOT[] = {B0000000,
                    B0000000,
                    B0000000,
                    B0000000,
                    B1100000,
                    B1100000,
                    B0000000,
                    B0000000};                     
                  
const int _COLDOT[] = {B0000000,
                       B0110000,
                       B0110000,
                       B0000000,
                       B0110011,
                       B0110011,
                       B0000000,
                       B0000000};                  

//CARREGA O MAPA DE BITS DE CARACTERE NUMA MATRIZ 
//CADA POSIÇÃO DE CARACTERE CORRESPONDE AO SEU INDICE DEFINIDO ANTERIORMENTE
//ISTO É _A (MAPA DE BITS DO "A") 
//SE O INDICE 0 É DO "A", ENTÃO LETTERS[A] RETORNARÁ O MAPA DE BITS DE "A")

const int* letters[] = {_A,_B,_C,_D,_E,_F,_G,_H,_I,_J,_K,_L,_M,_N,_O,_P,_Q,_R,_S,_T,_U,_V,_W,_X,_Y,_Z,_COL,_DASH,_BRA2,__, _FULL, _CHECK, _B2, _TEMP, _LINE, _SMILE, _DOT, _COLDOT};

void setup(){ 
  for(int i = 0; i <8; i++){  //PARA i IGUAL A 0, ENQUANTO i MENOR QUE 8, INCREMENTA i
    pinMode(rowA[i], OUTPUT); //DECLARA COMO SAÍDA
    pinMode(colA[i], OUTPUT);//DECLARA COMO SAÍDA
  }
}

void loop(){
 updateMatrix(); //ATUALIZA A MATRIZ COM INFORMAÇÃO A SER MOSTRADA
}

void updateMatrix(){
  loadSprite(); //CARREGA O MAPA DE BITS
  showSprite(velFrame); //MOSTRA O MAPA COM A VELOCIDADE DEFINIDO NA VARIÁVEL velFrame
}

//UMA MATRIZ CONTENDO A POTÊNCIA DE 2 USADA COMO MÁSCARA DE BITS PARA CALCULAR O QUE MOSTRAR
const int powers[] = {1,2,4,8,16,32,64,128};

//CARREGA O ESTADO ATUAL DO QUADRO A SER MOSTRADO NA MATRIZ DATA[]
void loadSprite(){
  int currentChar = getChar(msgExibida[index]); //LÊ O CARACTERE DA MATRIZ
  int nextChar = getChar(msgExibida[index+1]); //LÊ O PRÓXIMO CARACTERE
  
  for(int row=0; row < 8; row++){ //PARA row IGUAL A 0, ENQUANTO row MENOR QUE 8, INCREMENTA row
    data[row] = 0;                                   //RESETA A LINHA
    for(int column=0; column < 8; column++){ //PARA column IGUAL A 0, ENQUANTO column MENOR QUE 8, INCREMENTA column
     data[row] = data[row] + ((powers[column] & (letters[currentChar][row] << offset))); //CARREGA O CARACTERE ATUAL, COMPENSADO POR PIXELS DE DESLOCAMENTO
     data[row] = data[row] + (powers[column] & (letters[nextChar][row] >> (8-offset) ));  //CARREGA O PRÓXIMO CARACTERE COMPENSADO POR PIXELS DE DESLOCAMENTO 
    }
  }
  offset++; //INCREMENTA O OFFSET DE UMA LINHA
  
  if(offset==8){offset = 0; index++; if(index==sizeof(msgExibida)-2){index=0;}} //SE OFFSET É DE 8, CARREGAR O PAR DE CARACTERES A SEGUIR NA PRÓXIMA VEZ     
}

void showSprite(int velFrame2){
 for(int iii = 0; iii < velFrame2; iii++){ //PARA iii IGUAL A 0, ENQUANTO velFrame2 MENOR QUE iii, INCREMENTA iii
  for(int column = 0; column < 8; column++){ ///PARA column IGUAL A 0, ENQUANTO column MENOR QUE 8, INCREMENTA column
   for(int i = 0; i < 8; i++){ //PARA i IGUAL A 0, ENQUANTO i MENOR QUE 8, INCREMENTA i               
       digitalWrite(rowA[i], LOW); //DESLIGA TODOS OS PINOS DE LINHAS
   }
   for(int i = 0; i < 8; i++){ //PARA i IGUAL A 0, ENQUANTO i MENOR QUE 8, INCREMENTA i  
     if(i == column){     digitalWrite(colA[i], LOW);} //LIGA A LINHA CORRENTE
     else{                digitalWrite(colA[i], HIGH); } //LIGA O RESTANTE DAS LINHAS
   }

   for(int row = 0; row < 8; row++){ //PARA row IGUAL A 0, ENQUANTO row MENOR QUE 8, INCREMENTA row
    int bit = (data[column] >> row) & 1;
    if(bit == 1){ 
      digitalWrite(rowA[row], HIGH); //SE O BIT NA MATRIZ DE DADOS ESTÁ DEFINIDO, ACENDE O LED             
    }
   }
   delayMicroseconds(pauseDelay); //INTERVALO DE 500MS                    
  } 
 }
}

//RETORNA O INDICE DE UM DADO CARACTERE
//PARA A CONVERSÃO DE UMA STRING E PARA A PESQUISA NA MATRIZ DE MAPA DE BIT DE CARACTERES
int getChar(char charachter){
 int returnValue = Z;
 switch(charachter){
  case 'A': returnValue = A; break;
  case 'a': returnValue = A; break;
  case 'B': returnValue = B; break;
  case 'b': returnValue = B; break;
  case 'C': returnValue = C; break;
  case 'c': returnValue = C; break;
  case 'D': returnValue = D; break;
  case 'd': returnValue = D; break;
  case 'E': returnValue = E; break;
  case 'e': returnValue = E; break;
  case 'F': returnValue = F; break;
  case 'f': returnValue = F; break;
  case 'G': returnValue = G; break;
  case 'g': returnValue = G; break;
  case 'H': returnValue = H; break;
  case 'h': returnValue = H; break;
  case 'I': returnValue = I; break;
  case 'i': returnValue = I; break;
  case 'J': returnValue = J; break;
  case 'j': returnValue = J; break;
  case 'K': returnValue = K; break;
  case 'k': returnValue = K; break;
  case 'L': returnValue = L; break;
  case 'l': returnValue = L; break;
  case 'M': returnValue = M; break;
  case 'm': returnValue = M; break;
  case 'N': returnValue = N; break;
  case 'n': returnValue = N; break;
  case 'O': returnValue = O; break;
  case 'o': returnValue = O; break;
  case 'P': returnValue = P; break;
  case 'p': returnValue = P; break;
  case 'Q': returnValue = Q; break;
  case 'q': returnValue = Q; break;
  case 'R': returnValue = R; break;
  case 'r': returnValue = R; break;
  case 'S': returnValue = S; break;
  case 's': returnValue = S; break;
  case 'T': returnValue = T; break;
  case 't': returnValue = T; break;
  case 'U': returnValue = U; break;
  case 'u': returnValue = U; break;
  case 'V': returnValue = V; break;
  case 'v': returnValue = V; break;
  case 'W': returnValue = W; break;
  case 'w': returnValue = W; break;
  case 'X': returnValue = X; break;
  case 'x': returnValue = X; break;
  case 'Y': returnValue = Y; break;
  case 'y': returnValue = Y; break;
  case 'Z': returnValue = Z; break;
  case 'z': returnValue = Z; break;
  case ' ': returnValue = _; break;
  case '3': returnValue = B2; break;
  case '<': returnValue = TEMP; break;
  case '*': returnValue = FULL; break;
  case '|': returnValue = LINE; break;  
  case '_': returnValue = _; break;  
  case ':': returnValue = COL; break;  
  case '-': returnValue = DASH; break;  
  case ')': returnValue = BRA2; break;  
  case '%': returnValue = SMILE; break;  
  case '.': returnValue = DOT; break;    
  case '^': returnValue = COLDOT; break;      
  }
  return returnValue; //RETORNO DE INFORMAÇÃO
}