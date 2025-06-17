#include <AccelStepper.h>

const int stepsPerRevolution = 2048;  // altere isso para se ajustar ao número de etapas por revolução

// Pinos do driver do motor ULN2003
#define IN1 5
#define IN2 4
#define IN3 14
#define IN4 12

// inicializar a biblioteca de stepper
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  // inicializar a porta serial
  Serial.begin(115200);
  
  // definir a velocidade e a aceleração
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(100);
  // definir a posição do alvo
  stepper.moveTo(stepsPerRevolution);
}

void loop() {
  // verificar a posição atual do motor de passo para inverter a direção
  if (stepper.distanceToGo() == 0){
    stepper.moveTo(-stepper.currentPosition());
    Serial.println("Changing direction");
  }
  // mover o motor de passo (um passo de cada vez)
  stepper.run();
}