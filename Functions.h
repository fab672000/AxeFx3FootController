#pragma once
#include <Arduino.h>

void turnOffFlashingLed() {
  digitalWrite(leds[currentFlashLed], LOW );
}

void flashLed( int ledPin, int flashDelay ) {
  currentFlashLed = ledPin;
  digitalWrite(leds[ledPin], HIGH);
  timer.after(flashDelay, turnOffFlashingLed);
}

void lcdPrint(const char *text) {
  //lcd.clear();
  lcd.setCursor(0, 0); lcd.print(text);
}

void turnOffSceneLeds() {
  for (byte led = 0; led < MAX_SCENE_LED; led++) {
    digitalWrite(leds[led - 1], LOW);
  }
}

void turnOnSceneLed(byte led) {
  turnOffSceneLeds();
  digitalWrite(leds[led - 1], HIGH);
}

void doSceneChange(byte scene) {
  turnOnSceneLed(scene);
  Axe.sendSceneChange(scene);
  Axe.update();
}

void initButtons() {

  for (byte i = 0; i < NUM_BUTTONS; i++) {

    Buttons[i].setPin(ButtonPin[i]);
    Buttons[i].setDebounceTime(DEFAULT_DEBOUNCE);
    Buttons[i].setPullupEnable(true);
    Buttons[i].begin();

    pinMode( leds[i], OUTPUT );
    flashLed( leds[i], TAP_TEMPO_LED_DURATION );

  }

}

// Fetch All Scene Names

#define NUM_SCENES 8

//Struct to hold information about each scene
struct SceneInfo {
  SceneNumber number = -1;
  const char *name;
};

//A list of all of the scenes for current preset
SceneInfo scenes[NUM_SCENES];

void onPresetChanging(const PresetNumber number) {
  //Reset the scene list for the new preset
  for (byte i=0; i<NUM_SCENES; i++) {
    scenes[i].number = -1;
  }
}

/*void onPresetChange(AxePreset preset) {
  Serial.println("\nonPresetChange()\n=====================");
  Serial.print("Preset: ");
  Serial.print(preset.getPresetNumber());
  Serial.print(" - ");
  preset.printPresetName(Serial, true);

  Serial.print("Scene: ");
  Serial.print(preset.getSceneNumber());
  Serial.print(" - ");
  preset.printSceneName(Serial, true);
  Serial.println("=====================\n\n");
}
*/
void onSceneName(const SceneNumber number, const char* name, const byte length) {

  if (number == 1) {
    Serial.println("\n+++++++++++**+++++++++++\n\n");
  }

  Serial.print("onSceneName(): ");
  Serial.print(number);
  Serial.print(": ");
  Serial.println(name);

  //Record current scene in the list
  scenes[number-1].number = number;
  scenes[number-1].name = name;

  //Request the first scene that we don't have yet.
  //Only request one at a time to avoid filling up RX buffer
  for (byte i=0; i<NUM_SCENES; i++) {
    if (scenes[i].number == -1) {
      Axe.requestSceneName(i+1);
      break;
    }
  }

}
