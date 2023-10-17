#include "SettingsMgr.h"
#include "FcManager.h"
#include "MidiController.h"

#include <EEPROM.h>

static FootController* footController = nullptr;
// start reading from the first byte (address 0) of the EEPROM
constexpr int eeProtocolMagicNumber = 0;  // if not 0x42 then not initialized yet, default on Axe FX III protocol
constexpr int eeProtocolType = 1;

// Future: write calibration data
constexpr int eeCalibrationPedalBase = 4;
// ...

constexpr byte MagicNumber = 0x42;


static byte ButtonPin[NUM_BUTTONS] = {
#ifdef HAS_MUX
  BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,
  BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16,BUTTON1_16
#elif (BOARD == BOARD_SW16_EXP4) 
  BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8,
  BUTTON9, BUTTON10, BUTTON11, BUTTON12, BUTTON13, BUTTON14, BUTTON15, BUTTON16
#elif (BOARD == BOARD_MINI_TESTING) 
  BUTTON1, BUTTON2, BUTTON3, BUTTON4
#endif
};

FcDisplay SettingsManager::_display;
FcLeds SettingsManager::_leds;
FastMux SettingsManager::_mux(2, 3, 4, 5);
Bounce  SettingsManager::_buttons[NUM_BUTTONS];

SettingsManager::SettingsManager() {
  protocol = ProtocolType::MIDI_AFX3;
}

Bounce*  SettingsManager::getButtons() {
  return _buttons;
}

FastMux&  SettingsManager::getMux() {
  return  _mux;
}

FcLeds& SettingsManager::getLeds() {
    return _leds;
}
 
FcDisplay&  SettingsManager::getDisplay() {
  return _display;
}

void SettingsManager::initConfig(ProtocolType protocolType) {
  EEPROM.update(eeProtocolMagicNumber, MagicNumber);
  EEPROM.update(eeProtocolType, (int) protocolType);
  debugln(F("EEPROM is now initialized."));
}

void SettingsManager::initButtons() {
  for (byte i = 0; i < NUM_BUTTONS; i++) {
    pinMode(ButtonPin[i], INPUT_PULLUP);

#ifdef HAS_MUX
    _mux.select(i);
#endif
    _buttons[i].attach(ButtonPin[i], INPUT_PULLUP);
    _buttons[i].interval(DEFAULT_DEBOUNCE);
  }
}

void SettingsManager::begin() {
  // common initialization
  _mux.begin();
  initButtons();
  _leds.begin();
  _display.init();

#ifdef DEBUG
  Serial.begin(115200);
  while(!Serial);
#endif

  byte value = EEPROM.read(eeProtocolMagicNumber);

  if (value == MagicNumber) {
    debugln(F("magic number detected! Reading configuration ..."));

    value = EEPROM.read(eeProtocolType);
    protocol = (ProtocolType)(int)value;

    debug(F("Found foot controller protocol type: "));
    debugln(getControllerName(protocol));
  } else {
    debugln(F("no configuration detected, initialzing eeprom ..."));
    initConfig(protocol);
  }

  switch (protocol) {
    case MIDI_AFX3:
      footController = new FcManager();
      break;
    case MIDI_GM:
      footController = new MidiController();
      break;
    default:
      debugln(F("Unknow Controller type, falling back to AXE FX3."));
      protocol = ProtocolType::MIDI_AFX3;
      footController = new FcManager();
      initConfig(protocol);
      break;
  }
}

FootController* SettingsManager::getController() {
  return footController;
}

const char*  SettingsManager::getControllerName(ProtocolType protocol)
{
  switch(protocol) {
  case ProtocolType::MIDI_AFX3:
    return "AXE FX III";

  case ProtocolType::MIDI_GM:
    return "GENERAL MIDI";
  }
    return "UNKNOWN";
}
