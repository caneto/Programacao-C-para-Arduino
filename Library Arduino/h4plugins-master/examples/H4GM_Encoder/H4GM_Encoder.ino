#include<H4Plugins.h>
H4_USE_PLUGINS
/*
My major testing devices were nodeMCU which has builtin button on GPIO0 which is ACTIVE_LOW
and STM32NUCLEO-F429ZI whuch has a user button that is ACTIVE_HIGH

You will probably need to adjust these values for you own device
*/
#ifdef ARDUINO_ARCH_STM32
  #define UB_ACTIVE ACTIVE_HIGH
#else
  #define USER_BTN 0
  #define UB_ACTIVE ACTIVE_LOW
#endif
/*
    ALL GPIO strategies are derived from H4GPIOPin: the following members are available
    inside ALL GPIO pin callbacks, once you have a valid pointer for the pin type using 
    H4GM_PIN( type ) with specific underlying type Raw needs H4GM_PIN(Raw);

    uint8_t         pin=0;                  // GPIO hardware pin number
    uint8_t         gpioType=0;             // INPUT, INPUT_PULLUP, OUTPUT etc
    H4GM_STYLE      style;                  // Strategy: Raw, debounced, Encoder etc
    uint8_t         sense=0;                // active HIGH or LOW
    unsigned long   Tevt=0;                 // uS time of last event
    int             state=0;                // 32 wide as it holds analog value as well as digital 0/1
                                            // and not uint because encoder returns -1 or +1
    uint32_t        delta=0;                // uS since last change
    uint32_t        rate=0;                 // instantaenous rate cps
    uint32_t        Rpeak=0;                // peak rate   
    uint32_t        cps=0;                  // sigma changes/sec (used internally, frequently re-set)
    uint32_t        cMax=UINT_MAX;          // max permitted cps (see "throttling");
    uint32_t        nEvents=UINT_MAX;       // sigma "Events" (meaning depends on inheriting strategy)
            
    Additional members for Encoder:

		int             encoderValue // = +1 per clockwise click, -1 for anti-clockwise

        Manages a rotary encoder, including all necessary debouncing and decoding. You get called with 
        additional field `encoderValue` of -1 for every anti-clockwise click or +1 for clockwise. It's 
        that simple. Obviously it requires two physical pins but one single call does everything for you.

        It also has a variant that takes the name of an `int` variable instead of a callback: 
        the variable is incremented or decremented automatically when the encoder is turned.

*/
#define ENCODER_A D7
#define ENCODER_B D6

H4 h4(115200,20); //auto-start Serial @ 115200, Q size=20 
H4P_GPIOManager h4gm;

void h4setup() { // H4 constructor starts Serial
    Serial.println("H4P_GPIOManager Encoder Example v"H4P_VERSION);
    Serial.print("GPIO ");Serial.print(USER_BTN);Serial.print(" ACTIVE ");Serial.println(UB_ACTIVE ? "HIGH":"LOW");

    h4gm.Encoder(ENCODER_A,ENCODER_B,INPUT,UB_ACTIVE,[](H4GPIOPin* ptr){
        H4GM_PIN(Encoder); // Create the correct pointer type in 'pin'
        if(pin->encoderValue) {
            Serial.print("ENCODER 1 click ");Serial.print(pin->encoderValue < 0 ? "anti-":"");Serial.println("clockwise");
        } else Serial.println("ENCODER STATIC");// ==0 at startup
    });    
}