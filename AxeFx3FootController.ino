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

#include <LiquidCrystal_I2C.h>
#include <AxeFxControl.h>
#include <Timer.h>
#include "Hardware.h"
#include "Variables.h"
#include "Functions.h"
#include "ExpPedals.h"
//#include "SC_Button.h"
#include "Tuner.h"
#include "layout1.h"
#include "LayoutUser.h"
#include "LayoutSelect.h"
#include "lcd.h"

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#endif

  initLcd();

  // Setup Switches and activation LEDs
  initButtons();

  Axe.begin(Serial1);
  Axe.registerPresetChangeCallback(onPresetChange);
  // Axe.registerPresetChangingCallback(onPresetChanging);
  Axe.registerSystemChangeCallback(onSystemChange);
  Axe.registerSceneNameCallback(onSceneName);
  Axe.registerTunerStatusCallback(onTunerStatus);
  Axe.registerTunerDataCallback(onTunerData);
  Axe.registerTapTempoCallback(onTapTempo);
  Axe.enableRefresh(AXE_REFRESH_RATE);
  //Axe.requestPresetDetails();
  Axe.refresh(true);
}

void loop() {

  Axe.update();
  //timer.update();
  //expressionPedals();
  layout1();
  // layoutSelect();
}

void onPresetChange(AxePreset preset) {

  PresetNumb = (preset.getPresetNumber());
  preset.copyPresetName(bufPresetName, sz);
  SceNumb = (preset.getSceneNumber());
  preset.copySceneName(bufSceneName, sz);
  presetNameToLCD();
}

void onSystemChange() {
  // Display the current tempo at the LCD
  // lcd.setCursor(0, 3); lcd.print("Tempo: "); lcd.print(Axe.getTempo()); lcd.print("  ");
}

//this will only work if realtime sysex is enabled
void onTapTempo() {
  // Flashes a LED on tempo
  // flashLed( 3, TAP_TEMPO_LED_DURATION ); // pending assign to correspondent tempo led
}


void returnPreset(AxePreset preset) {

  if (Axe.isTunerEngaged() == false)
  {
    int PresetNumb = (preset.getPresetNumber());
    (Axe.sendPresetChange(PresetNumb));
  }

}
