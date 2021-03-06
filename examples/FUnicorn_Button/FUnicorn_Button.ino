/*

Welcome to the FUnicorn!
written by Carrie Sundra for Alpenglow Industries

This example sketch is for button-only activation, and uses a
low-power sleep mode for battery operation.

When you press the button, the FUnicorn executes a blink sequence.
It cycles through 5 different sequences.
The sequences are not interruptable, you must wait for one
sequence to end before activating the next one.

*/

#include <FUnicorn.h>

FUnicorn Fun;

volatile uint8_t buttJustPressed = 0;
volatile uint32_t buttTime = 0;

// records current time and sets buttJustPressed to 1 to start debounce timing
ISR(INT0_vect) {
  if (buttJustPressed == 0) {
    buttJustPressed = 1;
    buttTime = millis();
  }
}

////////////////////////////////////////////////////////////////////////////
// Button Handling
// - it's a heavy-handed stupid yet simple debounce routine
// - ignores all spurious bounces under the debounce time
// - when time is up, also checks to see if button is still pressed
//    (so it won't falsely trigger on button release bounces)
// - returns 1 for a valid button press, otherwise 0
////////////////////////////////////////////////////////////////////////////
uint8_t checkButt() {
  EIMSK &= ~(1 << INT0);        // disables INT0 to guarantee clearing buttJustPressed

  if (buttJustPressed && (millis() - buttTime > DEBOUNCE)) {
    buttJustPressed = 0;        // clears buttJustPressed to stop debounce timing
    EIMSK |= (1 << INT0);       // enables INT0
    if (BUTT_IS_PRESSED) return 1;
    else return 0;
  }

  EIMSK |= (1 << INT0);         // enables INT0
  return 0;
}


void setup() {

  // sets up the unicorn
  Fun.init();

  // initializes the button as an interrupt source, both wakes from sleep and triggers LEDs
  Fun.initButt();
  sei();

  // pulses the horn LED once to show that it's on
  Fun.hornBlink();

  // puts the unicorn to sleep until the button is pressed
  Fun.sleep();
  // wakes up from first button press here

}

void loop() {

    static uint16_t counter = 0;

    // checks validity of button press, when detected, executes a blink pattern
    if (checkButt()) {
      switch (counter % 5) {    // cycles through 5 patterns
        case 0:
        Fun.blinkDemo();
        break;
        case 1:
        Fun.blinkCrazy();
        break;
        case 2:
        Fun.FuckYouFuckFuckYou();
        break;
        case 3:
        Fun.blinkFuckYou2X();
        break;
        case 4:
        Fun.blinkAllOn();
        break;
      }
      counter++;
      delay(10);

      // goes to sleep after executing a blink pattern
      Fun.sleep();
      // wakes up from sleep here
    }
}
