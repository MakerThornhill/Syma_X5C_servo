#include <Adafruit_SoftServo.h>

/*******************************************************************
  SYMA X5C servo control by Thornhill
********************************************************************


  Uses the camera port on the Syma X5C to control a servo.
  When the picture or video buttons are pressed on the transmitter
  the signal wire (yellow) drops from 3.7v to 0v for 0.25s (picture)
  or 0.75s (video).

  Using those signals this sketch moves a servo from 0 degress to
  - 90 degrees

  Current config toggles between 0 and -90 degrees using a single
  button push (picture button), leaving one button open. Could
  be coded so one button increments servo in one direction, and the
  other button increments it the other.

  https://github.com/EThornill/Syma_X5C_servo

  ---------------------------------------------------------------------

  Board used: Adafruit Trinket (3v)
  Requires Adafruit_SoftServo library https://github.com/adafruit/Adafruit_SoftServo
  Tutorial: https://learn.adafruit.com/trinket-gemma-servo-control/overview

  ---------------------------------------------------------------------

  Detecting the camera signal was adapted from a 'hold button'
  sketch at http://playground.arduino.cc/Code/HoldButton

 *******************************************************************/

#define servoPin 0
int signal = 2; // The signal from the Syma camera port

int current;         // Current state of the signal
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
  pinMode(LED_BUILTIN, OUTPUT);

  OCR0A = 0xAF;            // any number is OK
  TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)

  myServo1.attach(servoPin);   // Attach the servo to pin 0 on Trinket
  myServo1.write(180);         // Initial position (horizontal)
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

      // PICTURE BUTTON (Low signal from Camera, 0.15 - 0.35s)
      if (millis_held > 150  && millis_held < 350) {

        //Move from 0 to -90 degrees (horizontal to straight down)
        if (toggle) {
          myServo1.write(0);
          toggle = !toggle;
          LEDblink (500);
        }
        //Move from 90 to 0 degrees (straight down to horizontal)
        else {
          myServo1.write(180);
          toggle = !toggle;
          LEDblink (500);
        }
      } //end_if picture

      // VIDEO BUTTON (low signal from Camera, ~0.75s)
      if (millis_held > 500) {
        //whatever we want... for now let's just show it works
        LEDblink (1500);
      }//end_if video

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

//----------------------------------------------------------
//----------------------------------------------------------

//Blink the board's LED to show it's working

void LEDblink(int LEDdelay) {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(LEDdelay);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}
