#include <TTP229.h> //INCLUSÃO DE BIBLIOTECA
 
const int SCL_PIN = 8;  //PINO DIGITAL UTILIZADO PELO TERMINAL SCL
const int SDO_PIN = 9;  //PINO DIGITAL UTILIZADO PELO TERMINAL SDO
 
TTP229 ttp229(SCL_PIN, SDO_PIN); //PASSA OS PARÂMETROS PARA A FUNÇÃO
 
void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  Serial.println("Pressione uma tecla..."); //IMPRIME O TEXTO NA SERIAL
  Serial.println(""); //QUEBRA DE LINHA NA SERIAL 
}
 
void loop(){
  uint8_t key = ttp229.ReadKey16(); //CAPTURA A TECLA PESSIONADA E ARMAZENA NA VARIÁVEL
  if (key){ //SE ALGUMA TECLA FOR PRESISONADA, FAZ
    Serial.print("Tecla pressionada: "); //IMPRIME O TEXTO NA SERIAL
    Serial.println(key); //IMPRIME NA SERIAL A TECLA PRESSIONADA
  }
}