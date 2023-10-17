#include "FootController.h"
#include "SettingsMgr.h"

// BUTTONS handling

FastMux&    FootController::_mux     = SettingsManager::getMux();
FcLeds&     FootController::_leds    = SettingsManager::getLeds();
FcDisplay&  FootController::_display = SettingsManager::getDisplay();
Bounce*     FootController::_buttons  = SettingsManager::getButtons();
SwitchHoldManager FootController::HoldMgr;

FootController::FootController(ProtocolType protocolType) 

{
  _protocolType = protocolType;
}

void FootController::begin()
{
  
  // now call virtual method that must be overriden by all concrete foot controllers
  init();
}


