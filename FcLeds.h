#pragma once
#include <Arduino.h>

/**
 * Leds handler class for the foot controller
 */
class FcLeds {
 public:
  static void begin();
  static void update();

  static void turnOffFlashingLed();
  static void flashLed( int ledPin, int flashDelay );
  static void turnOffSceneLeds();
  static void turnOnSceneLed(byte led);
  static void setTunerLed(bool state);
  static void setLooperLeds (int numLed);
};
