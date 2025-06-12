#include <LiquidCrystal_I2C.h>

// definir o número de colunas e linhas do LCD
int lcdColumns = 16;
int lcdRows = 2;

// definir o endereço do LCD, o número de colunas e linhas
// Se você não souber o endereço da tela, execute um esboço de scanner I2C
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  // inicializar o LCD
  lcd.begin();
  // ligar a luz de fundo do LCD                     
  lcd.backlight();
}

void loop(){
  // definir o cursor para a primeira coluna, primeira linha
  lcd.setCursor(0, 0);
  // imprimir mensagem
  lcd.print("Hello, World!");
  delay(1000);
  // limpa o visor para imprimir uma nova mensagem
  lcd.clear();
  // definir o cursor para a primeira coluna, segunda linha
  lcd.setCursor(0,1);
  lcd.print("Hello, World!");
  delay(1000);
  lcd.clear(); 
}