# Syma X5C servo
**Controllable servo using the Syma X5C's camera port &amp; an Arduino compatible micro-controller**

**...or anything else you want to control!**

 *******************************************************************

 Uses the camera port on the Syma X5C to control a servo. When the picture or video buttons are pressed on the transmitter the signal wire (yellow) drops from 3.7v to 0v for 0.25s (picture) or 0.75s (video).

Using those signals this sketch moves a servo from 0 degress to -90 degrees to tilt a camera from horizontal (normal FPV flight) to looking down (for landing). Current config just toggles between 0 and -90 degrees using a single button push (picture button), leaving one button open (use tbd). Could be coded so one button increments servo in one direction, and the other button increments it the other.

**Other uses**

The sketch can be adapted to control other components, such as LEDs, buzzers or altitude boards using the camera channel. Just make sure you include the code to read the camera signals. 

![alt text](https://raw.githubusercontent.com/EThornill/SYMA_X5C_servo/master/schematic.jpg "schematic")

 *******************************************************************
  **Board:** [Adafruit Trinket (3.3 v)](https://www.adafruit.com/product/1500)
  Lightweight (~1.85 grams), uses the same logic as the servo, and is Arduino compatible

  >"Required library is the [Adafruit_SoftServo library](https://github.com/adafruit/Adafruit_SoftServo). The standard Arduino IDE servo library will not work with 8 bit AVR microcontrollers like Trinket and Gemma due to differences in available timer hardware and programming. We simply refresh by piggy-backing on the timer0 millis() counter"

  *Tutorial:* https://learn.adafruit.com/trinket-gemma-servo-control/overview
  
  **Servo:** [HK-5320](https://hobbyking.com/en_us/hk-5320-ultra-micro-digital-servo-1-7g-0-05sec-0-075kg.html)
  This one is perfect because it's lightweight (< 2 grams) and works with an input voltage of 2.8 - 4.2 volts, which matches the Syma's 1s lipo battery.

 *******************************************************************

  Detecting the camera signal was adapted from a ['hold button' sketch](http://playground.arduino.cc/Code/HoldButton)

  Other bits and pieces were found in the Arduino forums

 *******************************************************************

**Demostration** 
- Project summary using a Trinket: https://youtu.be/Xz4V6qkwmkQ

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Xz4V6qkwmkQ
" target="_blank"><img src="http://img.youtube.com/vi/Xz4V6qkwmkQ/0.jpg" 
alt="Summary video on youtube" width="240" height="180" border="10" /></a>

- Initial test using a Pro Trinket: https://youtu.be/eBlpM11Wp7U



