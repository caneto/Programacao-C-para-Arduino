/*
  Programa de Variações de Variáveis
  Este firmware exibe mensagens sobre o Serial para entender melhor o uso das variáveis.
  Ligue a janela Serial Monitoring e reinicie a placa depois disso.
  Observe e verifique o código :)

*/

// declarando variáveis antes de se divertir!
boolean myBoolean;
char myChar;
int myInt;
float myFloat;
String myString;

void setup(){
  Serial.begin(9600);
  myBoolean = false;
  myChar = 'A';
  myInt = 1;
  myFloat = 5.6789 ;
  myString = "Ola humano!!";
}

void loop(){

  // verificando o booleano
  if (myBoolean) {
    Serial.println("myBoolean é verdade");
  } else {
      Serial.println("myBoolean é falso");
  }

  // jogando com char e int
  Serial.print("myChar está atualmente");
  Serial.write(myChar);
  Serial.println();

  Serial.print("myInt está atualmente ");
  Serial.print(myInt);

  Serial.println();
  Serial.print("Então, aqui está myChar + myInt:");
  Serial.write(myChar + myInt);
  Serial.println();
  
  // jogando com float e int
  Serial.print("myFloat é : ");
  Serial.print(myFloat);
  Serial.println();

  // colocar o conteúdo de myFloat em myInt
  myInt = myFloat;
  Serial.print("Coloquei meuFloat em meuInt, e aqui está meuInt agora:");
  Serial.println(myInt);

  // jogando com String
  Serial.print("myString é atualmente:");
  Serial.println(myString);

  myString += myChar; //concatenando String com Char
  Serial.print("myString tem um comprimento de");
  Serial.print(myString.length());// imprimindo o comprimento de myString
  Serial.print(" e é igual agora:");
  Serial.println(myString);

  // myString fica muito longa, mais de 15, removendo os últimos 3 elementos

  if (myString.length() >= 15){
     Serial.println("myString muito longa ... vamos, vamos limpar isso!");

     myInt = myString.lastIndexOf('!'); // encontrando o lugar do '!'
     myString = myString.substring(0,myInt+1);  // removendo personagens
    
   Serial.print("myString agora está mais limpo:");
     Serial.println(myString);
   
     // colocando true em myBoolean
  } else {
    myBoolean = false;     // redefinindo myBoolean para false
  }

  delay(5000);     // vamos fazer uma pausa
  
  // vamos colocar 2 linhas em branco para ter uma leitura clara
  Serial.println();
  Serial.println();
}
