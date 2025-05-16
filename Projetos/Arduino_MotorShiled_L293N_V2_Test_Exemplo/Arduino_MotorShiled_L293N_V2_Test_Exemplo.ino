#include <AFMotor.h>

// Número de etapas por rotação de saída
// Altere isso de acordo com a especificação do seu motor
const int stepsPerRevolution = 48;

// conectar o motor à porta nº 2 (M3 e M4)
AF_Stepper motor(stepsPerRevolution, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Stepper test!");

  motor.setSpeed(10);  // 10 rpm   
}

void loop() {
  Serial.println("Single coil steps");
  motor.step(100, FORWARD, SINGLE); 
  motor.step(100, BACKWARD, SINGLE); 

  Serial.println("Double coil steps");
  motor.step(100, FORWARD, DOUBLE); 
  motor.step(100, BACKWARD, DOUBLE);

  Serial.println("Interleave coil steps");
  motor.step(100, FORWARD, INTERLEAVE); 
  motor.step(100, BACKWARD, INTERLEAVE); 

  Serial.println("Micrsostep steps");
  motor.step(100, FORWARD, MICROSTEP); 
  motor.step(100, BACKWARD, MICROSTEP); 
}