#pragma once
#include "config.h"
#include "FcDisplay.h"
#include "FcLeds.h"
#include "FastMux.h"
#include "ProtocolType.h"
#include <Bounce2.h>
#include "SwitchHoldManager.h"

/// Base class for all controllers that this firmware will be able to handle
class FootController
{
public:
  FootController(ProtocolType protocolType);
  void begin(); // implements the Template Method design pattern
  ProtocolType getProtocolType() const { return _protocolType; }
  virtual void init() = 0;
  virtual void update() = 0;

  int sceneFromSwitchValue(int sw) {
  switch (sw) {
        case SWITCH_S1:         return 1;
        case SWITCH_S2:         return 2;
        case SWITCH_S3:         return 3;
        case SWITCH_S4:         return 4;
        case SWITCH_S5:         return 5;
        case SWITCH_S6:         return 6;
        case SWITCH_S7:         return 7;
        case SWITCH_S8:         return 8;
  }
  // should never happen, but if it bugs ; then use 1 instead of crashing
  return 1;
}

protected:
  ProtocolType _protocolType;
  static FcLeds& _leds;
  static FastMux& _mux;
  static FcDisplay& _display;
  static Bounce* _buttons;
  static SwitchHoldManager HoldMgr;

};