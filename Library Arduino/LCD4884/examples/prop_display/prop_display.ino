#include "LCD4884.h" 

#include "bildbmp.h"  
// relação x-y para pré-emparelhar imagens é 28 x 19 ~ 3 x 2
// porque os pixels do LCD na matriz de 84x48 parecem não ser quadráticos
// use uma imagem 3x2 de alta densidade, redimensione para 84x48 não promocional
// e mude para preto e branco
// conversor do google para bmp para pic para preparar o arquivo de inclusão

#define DELAY1 200
#define DELAY2 1000
#define DELAY3 5000

unsigned char text[] = "This is|Cap -|our|little|grand-|son.";

void setup()
{
  lcd.LCD_init(); // creates instance of LCD
}

void loop()
{  
   unsigned char i=0, line=0, pos=0, offset;
   unsigned char mychar;
   /*********** 1st ***********/
   lcd.LCD_clear(); // blanks the display
   delay(DELAY1);
   lcd.LCD_draw_bmp_pixel(0,0, bildbmp, 84,48);
   delay(DELAY3);
   lcd.LCD_set_XY(pos,line); //not necessary
   while (mychar=text[i++]) {
     if (mychar != '|') {
       lcd.LCD_prop_write_char(mychar, MENU_NORMAL);
     } else {
       line++;
       pos = 0;
       lcd.LCD_set_XY(pos,line);
     }
     delay(DELAY1);
   } 
   delay(DELAY3);
   /*********** 2nd ***********/
   lcd.LCD_clear(); // blanks the display
   delay(DELAY1);
   lcd.LCD_draw_bmp_pixel(0,0, bild2bmp, 84,48);
   delay(DELAY3);
   lcd.LCD_prop_write_string(0,0,"This",MENU_NORMAL);
   delay(DELAY2);
   lcd.LCD_prop_write_string(0,1,"is",MENU_NORMAL);
   delay(DELAY2);
   lcd.LCD_prop_write_string(0,2,"Trixi -",MENU_NORMAL);
   delay(DELAY2);
   lcd.LCD_prop_write_string(0,3,"his",MENU_NORMAL);
   delay(DELAY2);
   lcd.LCD_prop_write_string(0,4,"\"big\"",MENU_NORMAL);
   delay(DELAY2);
   lcd.LCD_prop_write_string(0,5,"sister.",MENU_NORMAL);
   delay(DELAY3);
}
