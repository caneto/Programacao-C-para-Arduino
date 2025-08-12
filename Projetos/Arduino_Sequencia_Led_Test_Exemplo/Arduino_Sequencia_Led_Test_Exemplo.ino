byte ledPin[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // cria um array para os pinos digitais dos LEDs
byte butPin = 2;
byte control = 0;
int ledDelay;
int direction;
int currentLED;
int ledDir;
int ledEsq;
int l;
int h;

void setup() {
  for (int x=0; x<10; x++) { // define todos os pinos dos LEDs como saída
    pinMode(ledPin[x], OUTPUT); 
  }
  pinMode(butPin, INPUT_PULLUP);
  Serial.begin(9600);  
}
    

void loop() {
  // muda de efeito
  if(!digitalRead(butPin)) {
    while (!digitalRead(butPin)) {} // espera soltar o botão
    delay(10); // reduz efeito bouncing
    control++;
    if(control>10) control = 1;
    Serial.println(control);
  }
  
  //cria repetições dos efeitos
  if (control==1) efeito1();
  if (control==2) efeito2();
  if (control==3) efeito3();
  if (control==4) efeito4();
  if (control==5) efeito5();
  if (control==6) efeito6();
  if (control==7) efeito7();
  if (control==8) efeito8();
  if (control==9) efeito9();
  if (control==10) efeito10();        
}


void efeito1() {
  // EFEITO 1 - vai e volta
      currentLED = 0;
      direction = 1;
      for(int i = 0; i < 72; i++) { // efeito 1: vai e volta
        digitalWrite(ledPin[currentLED], HIGH); // acende o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        delay(100);
        for (int x=0; x<10; x++) { // apaga todos os LEDs
          digitalWrite(ledPin[x], LOW);
        }
        // altera a direção se tivermos atingido o fim
        if (currentLED == 9) {direction = -1;}
        if (currentLED == 0) {direction = 1;}
        if(!digitalRead(butPin)) break; // interrompe o ciclo
    }
}

void efeito2() {
  // EFEITO 2 - girando
    currentLED = 0;
    direction = 1;
    for(int i = 0; i < 150; i++){ // efeito 2: girando
            
        digitalWrite(ledPin[currentLED], HIGH); // acende o LED atual
        if (currentLED>0){digitalWrite(ledPin[currentLED-1], LOW);} // acende o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        delay(35);
        if (currentLED == 10) {digitalWrite(ledPin[currentLED-1], LOW);currentLED = 0;}
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        }   
}

void efeito3() {
// EFEITO 3 - pares e ímpares    
    currentLED = 0;
    direction = 1;
    for(int i = 0; i < 84; i++){ // efeito 3: pares e ímpares       
        if (direction==1) {
        if ( currentLED % 2 == 0 ) {
        digitalWrite(ledPin[currentLED], HIGH); // acende o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        }
        else {
        digitalWrite(ledPin[currentLED], LOW); // apaga o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        }
        delay (65);
        }
        if (direction==-1){
        if ( currentLED % 2 == 0 ) {
        digitalWrite(ledPin[currentLED], LOW); // acende o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        }
        else {
        digitalWrite(ledPin[currentLED], HIGH); // apaga o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        }
        delay (65);
        }
        
        // altera a direção se tivermos atingido o fim
        if (currentLED == 10) {direction = -1;}
        if (currentLED == -1) {direction = 1;}
        if(!digitalRead(butPin)) break; // interrompe o ciclo
    }
}

void efeito4() {
  //EFEITO 4 - pares e ímpares 2
   for(int i = 0; i < 17; i++){ // efeito 4: pares e ímpares
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        for (int x=0; x<10; x++) { // ACENDE OS LEDs PARES
            if ( x % 2 == 0 ) {
            digitalWrite(ledPin[x], HIGH); // ACENDE o LED atual             
            }
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo       
        for (int x=0; x<10; x++) { // APAGA OS LEDs IMPARES
            if ( x % 2 != 0 ) {
            digitalWrite(ledPin[x], LOW); // apaga o LED atual             
            }
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo       
        delay(200);
        for (int x=0; x<10; x++) { // APAGA OS LEDs PARES
            if ( x % 2 == 0 ) {
            digitalWrite(ledPin[x], LOW); // apaga o LED par atual            
            } 
            digitalWrite(ledPin[x], LOW); // apaga o LED par atual   
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo 
        for (int x=0; x<10; x++) { // acende OS LEDs IMPARES
            if ( x % 2 != 0 ) {
            digitalWrite(ledPin[x], HIGH); // apaga o LED atual            
            }
            if(!digitalRead(butPin)) break; // interrompe o ciclo
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo      
        delay(200);       
    } 
       for (int x=0; x<10; x++) { // APAGA OS LEDs IMPARES
            if ( x % 2 != 0 ) {
            digitalWrite(ledPin[x], LOW); // apaga o LED atual            
            }
            if(!digitalRead(butPin)) break; // interrompe o ciclo
        }
        
        delay(100);        
}

void efeito5() {
  //EFEITO 5 - VU Meter
    currentLED = 0;
    direction = 1;
    for(int i = 0; i < 82; i++){ // efeito 5: Vu Meter
            
        if (direction==1) {
        digitalWrite(ledPin[currentLED], HIGH); // acende o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        delay(35);
        }
        if (direction==-1){
        digitalWrite(ledPin[currentLED], LOW); // acende o LED atual
        currentLED += direction; // incrementa de acordo com o valor de direction
        delay(35);
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        delay (200);
        // altera a direção se tivermos atingido o fim
        if (currentLED == 10) {direction = -1; currentLED=9;}
        if (currentLED == -1) {direction = 1; currentLED=0;}          
    }
}

void efeito6() {
  // EFEITO 6 - Giroflex
    for(int i = 0; i < 30; i++){
        digitalWrite(ledPin[0], HIGH);
        digitalWrite(ledPin[5], HIGH);
        delay(50);
        digitalWrite(ledPin[0], LOW);
        digitalWrite(ledPin[5], LOW);
        digitalWrite(ledPin[1], HIGH);
        digitalWrite(ledPin[6], HIGH);
        delay(50);
        digitalWrite(ledPin[1], LOW);
        digitalWrite(ledPin[6], LOW);
        digitalWrite(ledPin[2], HIGH);
        digitalWrite(ledPin[7], HIGH);
        delay(50);
        digitalWrite(ledPin[2], LOW);
        digitalWrite(ledPin[7], LOW);
        digitalWrite(ledPin[3], HIGH);
        digitalWrite(ledPin[8], HIGH);
        delay(50);
        digitalWrite(ledPin[3], LOW);
        digitalWrite(ledPin[8], LOW);
        digitalWrite(ledPin[4], HIGH);
        digitalWrite(ledPin[9], HIGH);
        delay(50);
        digitalWrite(ledPin[4], LOW);
        digitalWrite(ledPin[9], LOW);
        if(!digitalRead(butPin)) break; // interrompe o ciclo
    }
}

void efeito7() {
  // EFEITO 7
    for(int i = 0; i < 12; i++){
        for (int x=0; x<10; x++) { // acente todos os LEDs
        digitalWrite(ledPin[x], HIGH);
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        delay(150);

        for (int x=0; x<10; x++) { // APAGA OS LEDs PARES
            if ( x % 2 == 0 ) {
            digitalWrite(ledPin[x], LOW); // apaga o LED par atual
            }
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        delay(350);

        for (int x=0; x<10; x++) { // ACENDE OS LEDs PARES
            if ( x % 2 == 0 ) {
            digitalWrite(ledPin[x], HIGH); // ACENDE o LED atual
            }
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        delay(150);

        for (int x=0; x<10; x++) { // APAGA OS LEDs IMPARES
            if ( x % 2 != 0 ) {
            digitalWrite(ledPin[x], LOW); // apaga o LED atual
            }
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        delay(350);

        for (int x=0; x<10; x++) { // APAGA OS PARES
            if ( x % 2 == 0 ) {
            digitalWrite(ledPin[x], LOW); // apaga o LED atual
            }
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo
        delay(150);
    }
}

void efeito8() {
  // EFEITO 8 - Começa nos cantos
    ledDir = 0;
    ledEsq = 9;
    for(int i = 0; i < 4; i++){
        
        for(int i = 0; i < 5; i++){
            digitalWrite(ledPin[ledDir], HIGH); // acende o LED atual
            
            ledDir = ledDir +1;
            if(!digitalRead(butPin)) break; // interrompe o ciclo
            delay(182);
            
            digitalWrite(ledPin[ledEsq], HIGH); // acende o LED atual
            ledEsq = ledEsq -1;
            if(!digitalRead(butPin)) break; // interrompe o ciclo
            delay(182);
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo

         ledDir = 0;
         ledEsq = 9;   
         for(int i = 0; i < 5; i++){
            digitalWrite(ledPin[ledDir], LOW); // apaga o LED atual
            ledDir = ledDir +1;
            if(!digitalRead(butPin)) break; // interrompe o ciclo
            delay(182);
            
            digitalWrite(ledPin[ledEsq], LOW); // apaga o LED atual
            ledEsq = ledEsq -1;
            if(!digitalRead(butPin)) break; // interrompe o ciclo
            delay(182);
        }
      if(!digitalRead(butPin)) break; // interrompe o ciclo     
    }
}

void efeito9() {
  // EFEITO 9 - zig e zag
        
    for(int i = 0; i < 7; i++){
        
        l = 1; // apaga
        h = 0; // acende

        for(int i = 0; i < 10; i++){
            
            digitalWrite(ledPin[l], LOW); // apaga o LED atual
            l = l - 1;
            digitalWrite(ledPin[h], HIGH); // acende o LED atual           
            h = h + 2;
            if(!digitalRead(butPin)) break; // interrompe o ciclo  
            delay(90);
            
            digitalWrite(ledPin[l], LOW); // apaga o LED atual
            l = l + 2;
            digitalWrite(ledPin[h], HIGH); // acende o LED atual           
            h = h - 1;
            if(!digitalRead(butPin)) break; // interrompe o ciclo  
            delay(90);
        }
    }
}

void efeito10() {
  
// EFEITO 10 - três em três
        
    for(int i = 0; i < 7; i++){
        
        l = 7; // apaga
        h = 0; // acende

        for(int i = 0; i < 10; i++){
            
            digitalWrite(ledPin[l], LOW); // apaga o LED atual
            l = l + 1;
            if (l>9) {l = 0;}
            digitalWrite(ledPin[h], HIGH); // acende o LED atual           
            h = h + 1;
            if(!digitalRead(butPin)) break; // interrompe o ciclo  
            delay(103);         
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo  
        l = 7;
        for(int i = 0; i < 3; i++){
        digitalWrite(ledPin[l], LOW); // apaga o LED atual
        l = l + 1;
        if(!digitalRead(butPin)) break; // interrompe o ciclo  
        delay(103);
        }
        if(!digitalRead(butPin)) break; // interrompe o ciclo  
    }
}