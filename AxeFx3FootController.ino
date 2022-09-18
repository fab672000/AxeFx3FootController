// Axe-FX III midi.controller 
//   License is GPL V3, see LICENSE.MD for more details
// Displays preset/scene number and name on LCD
// Tuner working, but only graphics (moving arrows). Displaying the Note Name may eventually get corrupted data.
// More info: https://forum.fractalaudio.com/threads/diy-axe-fx-foot-controller-for-50-open-project.119882/
// Reusing tysonlt AxeFx Control Libraries:
//  https://github.com/tysonlt/AxeFxControl
// Other lib dependencies (for the LCD display and timer):
//  https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
//  https://github.com/JChristensen/Timer
//  https://github.com/thomasfredericks/Bounce2
// 
// by default, buttons are read from the 4067 on pins 2,3,4,5 and button pin is 6.
//    also data  LEDS 595 shifters are mapped on pins data = 7,clock = 8, latch = 9
// if not using 4067 and/or 595 shifters, comment out teh HAS_MUX and/or the HAS_LED_SHITERS definitions in config.h
//
#include "config.h"
#include "FcManager.h"
static FcManager footController(ProtocolType::MIDI_AFX3);

void setup() {
  footController.begin();
}

void loop() {
    footController.update();
}
