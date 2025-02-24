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

 for the filterAvarage() method:
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

// check that the library is not connected yet
#ifndef Button_h	// if the Button library is not connected
#define Button_h	// then connect it

#include "Arduino.h"

// signal processing class
class Button {
  public:
    Button(byte pin, byte timeButton);  // constructor
    boolean flagPress;    // sign button is pressed (low signal)
    boolean flagClick;    // sign of a button click (front)
    void  scanState();    // method waiting for a stable signal state
    void  filterAvarage(); // method of filtering the signal by the average value
    void setPinTime(byte pin, byte timeButton); // set the output number and filter time
	
  private:
    byte  _buttonCount;    // filtering time counter   
    byte _timeButton;      // filtering time
    byte _pin;             // pin number
};

#endif



