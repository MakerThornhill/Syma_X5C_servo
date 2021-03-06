#include <Adafruit_SoftServo.h>

/*******************************************************************
  SYMA X5C servo control - 3 position by Thornhill
********************************************************************


  Uses the camera port on the Syma X5C to control a servo.
  When the picture or video buttons are pressed on the transmitter
  the signal wire (yellow) drops from 3.7v to 0v for 0.25s (picture)
  or 0.75s (video).

  Using those signals this sketch moves a servo from 0 degress to 45 to
  -90 degrees

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
//boolean toggle = true;
byte servoPosition = 0;

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
      // Switch to horizontal mode - useful when you need to quickly look up
      // Using the picture button because this is the fastest

      if (millis_held > 150  && millis_held < 350) {
        myServo1.write(180); 
        servoPosition = 0;
      } //end_if picture

      // VIDEO BUTTON (low signal from Camera, ~0.75s)
      // Toggle between horizontal, 45 degrees, and straight down

      if (millis_held > 500) {
        servoPosition = (servoPosition + 1) % 3;

        switch (servoPosition) {
          case 0:
            myServo1.write(180);//Horizonal
            break;
          case 1:
            myServo1.write(90);//45 degree angle
            break;
          case 2:
            myServo1.write(0);//straight down
            break;
        }
        LEDblink (100);     
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

