/*-----------------------------------------------------------------------
 Programa : SHIFTOUTMEGA
 Descr    : Biblioteca para controlar portas adicionais OUTPUT do Arduino 
			utilizando o CI registrador de deslocamento 74HC595
 Autor    : Fellipe Couto [ http://www.efeitonerd.com.br ]
 Data     : 17/09/2012
 -----------------------------------------------------------------------*/
#ifndef ShiftOutMega_h  
#define ShiftOutMega_h  
#include "Arduino.h"
class ShiftOutMega{  
	public:
		ShiftOutMega(int latchPin, int dataPin, int clockPin, int qtdRegister);
		void shiftWrite(int pin, boolean state);
	private:
		int _latchPin;
		int _dataPin;
		int _clockPin;
		int _qtdRegister;
		void shiftOut(byte dataOut);
		String decToBin(int myNum);
		int binToDec(String b);
		int potency(int b, int e);
};
#endif