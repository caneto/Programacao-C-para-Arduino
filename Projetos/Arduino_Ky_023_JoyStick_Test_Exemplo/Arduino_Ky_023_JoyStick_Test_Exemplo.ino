int x = A0; //Saída analógica (Eixo X)
int y = A1; //Saída analógica (Eixo Y)
int botao = 2; //Saída digital do botão (Eixo Z)
char modo; //Variável para armazenar o caractere digitado pelo usuário

void setup() {
  Serial.begin(9600); //Inicializa a comunicação serial
  pinMode(botao, INPUT_PULLUP); //Define botao como entrada e em modo PULL UP, ou seja, lê HIGH quando o botão não estiver pressionado
}

void loop() {
  if (Serial.available()) { //Se a serial receber algum caractere
    modo = Serial.read(); //Lê o caractere e guarda na variável sentido
    Serial.println(modo); //Imprime na serial o conteúdo de sentido
  }
  //Seleção A imprime na serial a leitura analógica dos eixos e botão
  //Seleção B imprime na serial o movimento do bastão e botão do joystick
  switch (modo) {
    case 'A':
      leitura(); // Chama a função para leitura analógica dos eixos e botão
      break;

    case 'B':
      movimento(); // Chama a função para análise do movimento do bastão e do botão
      break;
  }
}

void leitura() {
  for (int i = 0; i < 10; i++) { //Laço para repetir 10 vezes as leituras dos eixos e botão
    Serial.print("Leitura do eixo X: ");
    Serial.println(analogRead(x));
    Serial.print("Leitura do eixo Y: ");
    Serial.println(analogRead(y));
    Serial.print("Leitura do botão: ");
    Serial.println(digitalRead(botao));
    Serial.println("------------------");
    delay(500);
  }
}

void movimento() {
  for (int j = 0; j < 10; j++) { //Laço para repetir 10 vezes as análises dos eixos e botão
    if ((analogRead(x)) == 0) {
      Serial.println("Bastão movimentado para baixo");
    }
    if ((analogRead(x)) == 1023) {
      Serial.println("Bastão movimentado para cima");
    }
    if ((analogRead(y)) == 0) {
      Serial.println("Bastão movimentado para esquerda");
    }
    if ((analogRead(y)) == 1023) {
      Serial.println("Bastão movimentado para direita");
    }
    if ((digitalRead(botao)) == 0) {
      Serial.println("Botão pressionado");
    }
    if ((digitalRead(botao)) == 1) {
      Serial.println("Botão solto");
    }
    Serial.println("------------------");
    delay(500);
  }
}