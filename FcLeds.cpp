#include "config.h"
#include "FcLeds.h"
#include <Timer.h>

Timer timer;

// Initial state of switch is high due to internal pullup
const byte PROGMEM leds[NUM_LEDS] = {
#if (BOARD == BOARD_SW16_EXP4) 
  LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10, LED11, LED12, LED13, LED14, LED15, LED16
#elif (BOARD == BOARD_MINI_TESTING) 
  LED1, LED2, LED3, LED4
#endif
 };
 
static byte _currentFlashLed; // used for switching flashing off

void FcLeds::update() {
  timer.update();
}

void FcLeds::turnOffFlashingLed() {
  digitalWrite(leds[_currentFlashLed], LOW );
}

void FcLeds::flashLed( int ledNum, int flashDelay ) {
  _currentFlashLed = ledNum;
  digitalWrite(leds[ledNum], HIGH);
  timer.after(flashDelay, turnOffFlashingLed);
}

void FcLeds::turnOffSceneLeds() {
  for (byte led = 0; led < MAX_SCENE_LED; led++) {
    digitalWrite(leds[led - 1], LOW);
  }
}

void FcLeds::turnOnSceneLed(byte sceneNum) {
  turnOffSceneLeds();
  digitalWrite(leds[sceneNum - 1], HIGH);
}

void FcLeds::begin()
{
  for (byte i = 0; i < NUM_LEDS; i++) {
    pinMode( leds[i], OUTPUT );
    flashLed( leds[i], TAP_TEMPO_LED_DURATION );
  }
}

void FcLeds::setTunerLed(bool state) {
#if NUM_LEDS >=15
  digitalWrite(LED15, state ? HIGH : LOW);
#endif
}

void FcLeds::setLooperLeds (int numLed) {
#if NUM_LEDS >= 13
    digitalWrite( LED11, numLed == 1 ? HIGH : LOW);
    digitalWrite( LED12, numLed == 2 ? HIGH : LOW);
    digitalWrite( LED13, numLed == 3 ? HIGH : LOW);
#endif

}
