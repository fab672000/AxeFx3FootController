#include "config.h"
#include "FcLeds.h"
#include <Timer.h>

static Timer timer;

// Initial state of switch is high due to internal pullup
const byte PROGMEM leds[NUM_LEDS] = {
#if (BOARD == BOARD_SW16_EXP4) 
  LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10, LED11, LED12, LED13, LED14, LED15, LED16
#elif (BOARD == BOARD_MINI_TESTING) 
  LED1, LED2, LED3, LED4
#endif
 };
 
static byte _currentFlashLed; // used for switching flashing off
uint16_t FcLeds::_ledsState = 0;

FcLeds::FcLeds()
{
  
#ifdef HAS_LED_SHIFTERS
  _ledsState = 0;
  updateLeds(_ledsState);
#endif
}

void FcLeds::update() {
  timer.update();
}

void FcLeds::begin()
{
#ifndef HAS_LED_SHIFTERS
  for (byte i = 0; i < NUM_LEDS; i++) {
    pinMode( leds[i], OUTPUT );
    flashLed( leds[i], TAP_TEMPO_LED_DURATION );
  }
#else
  pinMode(LED_DATA, OUTPUT);
  pinMode(LED_CLOCK, OUTPUT);
  pinMode(LED_LATCH, OUTPUT);
  testAllLeds(250);
#endif
}

void FcLeds::updateLed(byte ledNum, byte  state)
{
#ifdef HAS_LED_SHIFTERS
  uint16_t bitVal = (1 << ledNum);
  auto prevLedState = (_ledsState & bitVal) != 0 ? HIGH : LOW;
  
  if (prevLedState == state) return;
  _ledsState = state ? _ledsState | bitVal : _ledsState & ~bitVal; 
  updateLeds(_ledsState);
#else
  digitalWrite(leds[ledNum], state);
#endif
}

void FcLeds::updateLeds(uint16_t val)
{
    static uint16_t lastLedVal=0;

    if (val!=lastLedVal)
    {
      lastLedVal = val;
#ifdef DEBUG
        Serial.print("Updating Leds: ");
        Serial.println(val,HEX);
#endif
    }
    
    digitalWrite(LED_LATCH, LOW);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, (val>>8) & 0xff);  // MSB
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, val & 0xff);       // LSB
    //take the latch pin high so that all the LEDs will update:    
    digitalWrite(LED_LATCH, HIGH);
}

void FcLeds::testAllLeds(uint16_t del)
{
  for (byte n=0; n<3; n++)
  {
    for (uint16_t  i= 0; i< NUM_LEDS; i++) {
      updateLed(i, i & 1 ? HIGH : LOW);
    }
    delay(del);

    for (uint16_t  i= 0; i< NUM_LEDS; i++) {
      updateLed(i, i & 1 ? LOW : HIGH);
    }  
    delay(del);
  }
    updateLeds(0);
}void FcLeds::turnOffFlashingLed() {
  updateLed(_currentFlashLed, LOW );
}

void FcLeds::flashLed( byte ledNum, int flashDelay ) {
  _currentFlashLed = ledNum;
  updateLed(ledNum, HIGH);
  timer.after(flashDelay, turnOffFlashingLed);
}

void FcLeds::turnOffSceneLeds() {
  for (byte led = 0; led < MAX_SCENE_LED; led++) {
    updateLed(led, LOW);
  }
}

void FcLeds::turnOnSceneLed(byte sceneNum) {
  turnOffSceneLeds();
  updateLed(sceneNum - 1, HIGH);
}

void FcLeds::setTunerLed(bool state) {
#if NUM_LEDS >=15
  updateLed(LED15, state ? HIGH : LOW);
#endif
}

void FcLeds::setLooperLeds (int numLed) {
#if NUM_LEDS >= 13
    updateLed(LED11, numLed == 1 ? HIGH : LOW);
    updateLed(LED12, numLed == 2 ? HIGH : LOW);
    updateLed(LED13, numLed == 3 ? HIGH : LOW);
#endif
}
