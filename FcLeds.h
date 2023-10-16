#pragma once
#include <Arduino.h>
/**
 * Leds handler class for the foot controller
 */
class FcLeds {
 public:
  FcLeds();

  void begin();
  void update();

  void flashLed( byte ledPin, int flashDelay );
  void turnOffSceneLeds(); 
  void turnOnSceneLed(byte led); // scene value is : 1-8
  void setTunerLed(bool state);
  void setLooperLeds (int numLed);
  
  void testAllLeds(uint16_t del);

  static void updateLed(byte ledNum, byte state);
  static void turnOffFlashingLed(); // must be static because of timer cb limitation
  static void updateLeds(uint16_t value);
 private:
  static uint16_t _ledsState; // value of all leds 0=off, 1=on
};
