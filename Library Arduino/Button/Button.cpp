/*
Button.h - a library for digitally processing the signals of the button contacts
and signals of other components by a parallel process.
 
In a parallel process, one of the following methods should be called regularly:
    void  scanState();    // method waiting for a stable signal state
    void  filterAvarage(); // method of filtering the signal by the average value

As a result, signs are formed:
 
 for the scanState() method:
 - when the button is pressed flagPress = true
 - when the button is free flagPress = false
 - when the button was clicked flagClick = true

 for the filterAvarage () method:
 - with a low signal flagPress = true
 - with high signal flagPress = false
 - when the state changes from high to low flagClick = true

An object of type Button, when created, has parameters:

 - pin number to which the button or signal is connected
 - signal processing time (multiplied by the period of calling the scanState() method or filterAvarage()

Button button1 (12, 15); // create an object for the button connected to the 12 pin
 with a filter time of 30 ms (with a cycle of 2 ms)
The library developed by Kalinin Edward

http://mypractic.com/lesson-8-digital-filtering-of-signals-in-arduino-programs/  
*/

#include "Arduino.h"
#include "Button.h"


// method of filtering the signal by average value
// for a low-level signal flagPress = true
// for a high-level signal flagPress = false
// when the state changes from high to low flagClick = true
void Button::filterAvarage() {

 if ( flagPress != digitalRead(_pin) ) {
     //  button state remains the same
     if ( _buttonCount != 0 ) _buttonCount--; // counter of acknowledgments - 1 with limitatuon to 0   
  }
  else {
     // button state changed
     _buttonCount++;   // +1 to the acknowledge counter

     if ( _buttonCount >= _timeButton ) {
      // the signal state reached the threshold _timeButton
      flagPress= ! flagPress; // inversion of the state sign
     _buttonCount= 0;  // reset acknowledge counter

      if ( flagPress == true ) flagClick= true; // sign of the button click       
     }    
  }
}


// method for checking the status of the button
// when the flagPress = true button is pressed
// when the flagPress = false is free
// when you click flagClick = true
void Button::scanState() {

 if ( flagPress != digitalRead(_pin) ) {
     // flagPress flag = current button state 
     // (inversion because the active state of the LOW button) 
     // i.e. button state remains the same 
     _buttonCount= 0;  // reset acknowledge counter
  }
  else {
     // flagPress flag not = current button state 
     // button state changed
     _buttonCount++;   // +1 to the acknowledge counter

     if ( _buttonCount >= _timeButton ) {
      // the state of the button did not change for the time _timeButton
      // the state of the button became stable
      flagPress= ! flagPress; // inversion of the state sign
     _buttonCount= 0;  // reset acknowledge counter

      if ( flagPress == true ) flagClick= true; // sign of the button click      
     }    
  }
}


// method of setting pin number and time of acknowledgment
void Button::setPinTime(byte pin, byte timeButton)  {

  _pin= pin;
  _timeButton= timeButton;
  pinMode(_pin, INPUT_PULLUP);  // define the pin of the button as an input
}

// описание конструктора класса Button
Button::Button(byte pin, byte timeButton) {

  _pin= pin;
  _timeButton= timeButton;
  pinMode(_pin, INPUT_PULLUP);  // define the pin of the button as an input
}