/*******************************************************************
  SYMA X5C servo control
*******************************************************************

  Uses the camera port on the SYMA X5C to control a servo.
  When the picture or video buttons are pressed on the transmitter
  the signal wire (yellow) drops from 3.7v to 0v for 0.25s (picture)
  or 0.75s (video).

  Using those signals this sketch moves a servo from 0 degress to
  - 90 degrees to tilt a camera from horizontal (normal FPV flight)
  to looking down (for landing).
  
  Current config just toggles between 0 and -90 degrees using a single 
  button push (picture button), leaving one button open (use tbd). Could 
  be coded however so one button increments servo in one direction, and the 
  other button increments it the other.

 *******************************************************************
  Board: Adafruit Trinket (3v)
  Notes:

  "Required library is the Adafruit_SoftServo library
  available at https://github.com/adafruit/Adafruit_SoftServo
  The standard Arduino IDE servo library will not work with 8 bit
  AVR microcontrollers like Trinket and Gemma due to differences
  in available timer hardware and programming. We simply refresh
  by piggy-backing on the timer0 millis() counter"

  Tutorial: https://learn.adafruit.com/trinket-gemma-servo-control/overview

 *******************************************************************

  Detecting the camera signal was adapted from a 'hold button'
  sketch at http://playground.arduino.cc/Code/HoldButton

  Other bits and pieces were found in the Arduino forums

 *******************************************************************/
 
#include <Adafruit_SoftServo.h>

#define SERVO1PIN 0   // Servo control line on Trinket Pin #7
int signal = 2; // The signal from the SYMA camera port
int LEDsignal = 1; // Blink

int current;         // Current state of the button
                     // (LOW is pressed b/c i'm using the pullup resistors)
long millis_held;    // Duration of signal (milliseconds)
long secs_held;      // Duration of signal (seconds)
long prev_secs_held; // Duration previously
byte previous = HIGH;
unsigned long firstTime; // how long since the signal was detected


Adafruit_SoftServo myServo1;
boolean toggle = true;

void setup() 
{ 
  digitalWrite(signal, HIGH);
  pinMode(LEDsignal, OUTPUT);
  digitalWrite(LEDsignal, LOW);
  
  
 OCR0A = 0xAF;            // any number is OK
 TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)
  
  myServo1.attach(SERVO1PIN);   // Attach the servo to pin 0 on Trinket
  myServo1.write(165);          // Initial position (horizontal)
  delay(15);                   // Wait 15ms for the servo to reach the position
}

void loop() {
  current = digitalRead(signal);
  
  //--------------- Reading the camera signal ----------------
  //----------------------------------------------------------

  // if the state changes to low signal, remember the start time 
  if (current == LOW && previous == HIGH && (millis() - firstTime) > 200) {
    firstTime = millis();
  }

  millis_held = (millis() - firstTime);
  secs_held = millis_held / 1000;

  //debouncing tool... the low signal must have a duration for at least 100 milliseconds to be considered
  if (millis_held > 100) {
   
    // check that the low signal returned to high since we last checked
    if (current == HIGH && previous == LOW) {
      
      //----------------- Controlling the Servo ------------------
      //----------------------------------------------------------
 
      // Low signal from Camera, 0.15 - 0.35s (picture button)
      if (millis_held > 150  && millis_held < 350) { 
        
        //Move from 0 to -90 degrees (horizontal to straight down)
        if(toggle){
          myServo1.write(70);
          toggle = !toggle;
          digitalWrite(LEDsignal, HIGH);   // turn the LED on
          delay(500);                       // wait for half a second
          digitalWrite(LEDsignal, LOW);    // turn the LED off
        }
        //Move from 90 to 0 degrees (straight down to horizontal)
        else{
          myServo1.write(165);
          toggle = !toggle;
          digitalWrite(LEDsignal, HIGH);   // turn the LED on
          delay(500);                       // wait for half a second
          digitalWrite(LEDsignal, LOW);    // turn the LED off
        }
      } //0.25s end_if
      
     // Low signal from Camera is >0.5s (video button)
      if (millis_held > 500) {
        //whatever we want... haven't decided yet
      }//0.75s end_if
 
      //----------------------------------------------------------
      //----------------------------------------------------------
      
      }
  }

  previous = current;
  prev_secs_held = secs_held;
} //end loop

//----------------------------------------------------------
//----------------------------------------------------------
      
// We'll take advantage of the built in millis() timer that goes off
      // to keep track of time, and refresh the servo every 20 milliseconds
      // The SIGNAL(TIMER0_COMPA_vect) function is the interrupt that will be
      // Called by the microcontroller every 2 milliseconds

    volatile uint8_t counter = 0;
    SIGNAL(TIMER0_COMPA_vect) {
      // this gets called every 2 milliseconds
        counter += 2;
        // every 20 milliseconds, refresh the servos!
        if (counter >= 20) {
        counter = 0;
        myServo1.refresh();
            }
        }  
