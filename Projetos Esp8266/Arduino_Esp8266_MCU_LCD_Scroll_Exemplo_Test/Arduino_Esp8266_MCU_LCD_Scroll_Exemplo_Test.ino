#include <LiquidCrystal_I2C.h>

// definir o número de colunas e linhas do LCD
int lcdColumns = 16;
int lcdRows = 2;

// definir o endereço do LCD, o número de colunas e linhas
// Se você não souber o endereço da tela, execute um sketcho de scanner I2C
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

String messageStatic = "Static message";
String messageToScroll = "This is a scrolling message with more than 16 characters";

// Função para rolar o texto
// A função aceita os seguintes argumentos:
// row: número da linha em que o texto será exibido
// message: mensagem a ser rolada
// delayTime: atraso entre cada deslocamento de caractere
// lcdColumns: número de colunas do seu LCD
void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup(){
  // inicializar o LCD
  lcd.begin();
  // ligar a luz de fundo do LCD                      
  lcd.backlight();
}

void loop(){
  // definir o cursor para a primeira coluna, primeira linha
  lcd.setCursor(0, 0);
  // imprimir mensagem estática
  lcd.print(messageStatic);
  // imprimir mensagem de rolagem
  scrollText(1, messageToScroll, 250, lcdColumns);
}