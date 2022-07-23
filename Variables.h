#pragma once
#include "SC_Button.h"
String txt;
byte ButtonPin[NUM_BUTTONS] = {
  BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8,
  BUTTON9, BUTTON10, BUTTON11, BUTTON12, BUTTON13, BUTTON14, BUTTON15, BUTTON16
};

// Initial state of switch is high due to internal pullup
byte leds[NUM_LEDS] = {
  LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9, LED10, LED11, LED12, LED13, LED14, LED15, LED16
};

Button Buttons[NUM_BUTTONS];

/* LCD definition
   lcd(RS, Enable, D4, D5, D6, D7, BL)
   KEY pin to analogl pin 0
*/
//LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

AxeSystem Axe;
Tempo tempo;
Timer timer;

byte currentFlashLed;
byte MidiChannel = 1;
int PresetNumb;
byte SceNumb;
const size_t sz = 150;
char buf[sz];
char bufPresetName[sz];
char bufSceneName[sz];

//commands
enum Command {
  PRESET_DOWN,
  PRESET_UP,
  SCENE_DOWN,
  SCENE_UP,
  SCENE_CHANGE,
  EFFECT_TOGGLE,
  MIDI_PC,
  MIDI_CC,
  __COMMAND_COUNT
};
