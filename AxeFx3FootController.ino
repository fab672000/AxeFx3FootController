// Axe-FX III midi.controller 19-May-2019 20x4 I2C LCD Display Version
//   License is GPL V3, see LICENSE.MD for more details
// Displays preset/scene number and name on LCD
// Tuner working, but only graphics (moving arrows). Displaying the Note Name may eventually get corrupted data.
// More info: https://forum.fractalaudio.com/threads/diy-axe-fx-foot-controller-for-50-open-project.119882/
// Reusing tysonlt AxeFx Control Libraries:
//  https://github.com/tysonlt/AxeFxControl
//  https://github.com/tysonlt/Simple_Controls
// Other lib dependencies (for the LCD display and timer):
//  https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
//  https://github.com/JChristensen/Timer

#define DEBUG

#include <Timer.h>
#include "config.h"
#include "FcManager.h"

static FcManager prot(ProtocolType::MIDI_AFX3);

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  prot.begin();
}

void loop() {
  prot.update();
  // timer.update();
}
