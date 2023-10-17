#pragma once
#include "config.h"
#include "ProtocolType.h"
#include "FootController.h"


class SettingsManager
{
public:
  SettingsManager();

  void begin();
  FootController* getController();
  static const char * getControllerName(ProtocolType protocol);

  static Bounce* getButtons();
  static FastMux& getMux();
  static FcLeds& getLeds();
  static FcDisplay& getDisplay();

private:
  static void initButtons();
  void initConfig(ProtocolType protocolType);

  static FcDisplay _display;
  static FcLeds _leds;
  static FastMux _mux;
  static Bounce _buttons[NUM_BUTTONS];
  ProtocolType protocol;
};