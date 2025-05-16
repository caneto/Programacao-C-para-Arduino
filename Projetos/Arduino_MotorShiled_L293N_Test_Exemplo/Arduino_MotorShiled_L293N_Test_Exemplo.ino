#include <AFMotor.h>

AF_DCMotor motor(4);

void setup() 
{
	//Defina a velocidade inicial do motor e pare
	motor.setSpeed(200);
	motor.run(RELEASE);
}

void loop() 
{
	uint8_t i;

	// Ligar o motor
	motor.run(FORWARD);
	
	// Acelerar de zero a velocidade máxima
	for (i=0; i<255; i++) 
	{
		motor.setSpeed(i);  
		delay(10);
	}
	
	// Desacelerar da velocidade máxima para zero
	for (i=255; i!=0; i--) 
	{
		motor.setSpeed(i);  
		delay(10);
	}

	// Agora mude a direção do motor
	motor.run(BACKWARD);
	
	// Acelerar de zero a velocidade máxima
	for (i=0; i<255; i++) 
	{
		motor.setSpeed(i);  
		delay(10);
	}

	// Desacelerar da velocidade máxima para zero
	for (i=255; i!=0; i--) 
	{
		motor.setSpeed(i);  
		delay(10);
	}

	// Agora desligue o motor
	motor.run(RELEASE);
	delay(1000);
}