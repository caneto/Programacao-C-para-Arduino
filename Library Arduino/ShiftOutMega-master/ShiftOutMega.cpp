/*-----------------------------------------------------------------------
 Programa : SHIFTOUTMEGA
 Descr    : Biblioteca para controlar portas adicionais OUTPUT do Arduino 
			utilizando o CI registrador de deslocamento 74HC595
 Autor    : Fellipe Couto [ http://www.efeitonerd.com.br ]
 Data     : 17/09/2012
 -----------------------------------------------------------------------*/
#include "Arduino.h"
#include "ShiftOutMega.h"
String pShift[100] = {};
int _dataPin;
int _clockPin;
ShiftOutMega::ShiftOutMega(int latchPin, int dataPin, int clockPin, int qtdRegister){
	pinMode(latchPin, OUTPUT);
	_latchPin = latchPin;
	pinMode(dataPin, OUTPUT);
	_dataPin = dataPin;
	pinMode(clockPin, OUTPUT);
	_clockPin = clockPin;
	_qtdRegister = qtdRegister;
	for (int i=0; i<=_qtdRegister; i++){
		pShift[i] = "00000000";
	}
	for (int i=1; i<=_qtdRegister * 8; i++){
		shiftWrite(i, LOW);
	}
}
void ShiftOutMega::shiftWrite(int pin, boolean state){
	int countShift = 1;
	int data = 256;
	for (int i=1; i<=pin; i++){
		if (data <= 1){
			data = 256;
			countShift++;
		}
		data /= 2;
	}
	String dataAux;
	int pinAux = pin - (8 * (countShift - 1));
	dataAux += pShift[countShift].substring(0, pinAux-1);
	if (state == HIGH){
		dataAux += "1";
	}
	else{
		dataAux += "0"; 
	}
	if (pinAux < 8){
		dataAux += pShift[countShift].substring(pinAux, 8);
	}
	data = binToDec(dataAux);
	digitalWrite(_latchPin, LOW);
	for (int i=_qtdRegister; i>0; i--){
		if (i == countShift){
			shiftOut(data);
			pShift[i] = dataAux;
		}
		else{
			shiftOut(binToDec(pShift[i]));
		}
	}
	digitalWrite(_latchPin, HIGH);
}
int ShiftOutMega::potency(int b, int e){
	if(e == 0){
		return 1;
	}
	else{   
		return b * potency(b, e - 1);
	}
}
void ShiftOutMega::shiftOut(byte dataOut){
	boolean pinState;
	digitalWrite(_dataPin, LOW);
	digitalWrite(_clockPin, LOW);
	for (int i=0; i<8; i++){
		digitalWrite(_clockPin, LOW);
		if (dataOut & (1<<i)){
			pinState = HIGH;
		}
		else{
			pinState = LOW;
		}
		digitalWrite(_dataPin, pinState);
		digitalWrite(_clockPin, HIGH);
	}
	digitalWrite(_clockPin, LOW);
}
String ShiftOutMega::decToBin(int myNum){
	int zeros = 8 - String(myNum,BIN).length();
	String myStr;
	for (int i=0; i<zeros; i++) {
		myStr = myStr + "0";
	}
		myStr = myStr + String(myNum,BIN);
	return myStr;
}
int ShiftOutMega::binToDec(String b){
	int dec = 0;
	int exponent = 0;
	for (int i=8; i>0; i--){
		dec += b.substring(i-1, i).toInt() * potency(2, exponent);
		exponent++;
	}
	return dec;
}