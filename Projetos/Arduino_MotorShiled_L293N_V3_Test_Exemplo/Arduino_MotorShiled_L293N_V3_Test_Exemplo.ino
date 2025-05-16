#include <Servo.h> 

Servo myservo;	// criar um objeto servo para controlar um servo
int pos = 0;	// variável para armazenar a posição do servo

void setup() 
{
	// conecta o servo no pino 10 ao objeto servo
	myservo.attach(10);   
}

void loop() 
{
	// varre de 0 graus a 180 graus
	for(pos = 0; pos <= 180; pos += 1) 
	{
		myservo.write(pos);
		delay(15);
	}
	// varre de 180 graus a 0 graus
	for(pos = 180; pos>=0; pos-=1)
	{
		myservo.write(pos);
		delay(15);
	}
}