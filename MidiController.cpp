#include "MidiController.h"

MidiController::MidiController() : 
  FootController(MIDI_GM), 
  serialMIDI(*(new midi::SerialMIDI<HardwareSerial>(Serial1)) ), 
  MIDI(*(new midi::MidiInterface<midi::SerialMIDI<HardwareSerial>>(serialMIDI)) )
{
  _scene = 0;
  _bank = 0;
}

void MidiController::init()
{

}

void MidiController::sendProgramChange(byte program)
{
  MIDI.sendProgramChange(program, 1);
}

void MidiController::sendControlChange(byte control, byte value)
{
  MIDI.sendControlChange(control, value, 1);
}

void MidiController::sendBankChange(byte bank)
{
  MIDI.sendControlChange(0, bank, 1);
}

void MidiController::update()
{
    _leds.update(); // update the timer

}

void MidiController::handleEvents()
{
    long nowTimeMs = millis();
  
  for (byte switchIndex = 0; switchIndex < NUM_BUTTONS; switchIndex++) {
#ifdef HAS_MUX
    _mux.select(switchIndex);
#endif
    auto& currentButton =  _buttons[switchIndex];
    currentButton.update();
    
    // handle hold durations
    const auto  fellState = currentButton.fell();
    const auto  roseState = currentButton.rose();
    HoldMgr.update(switchIndex, fellState, roseState, nowTimeMs);
        const auto  changed =  HoldMgr.hasChanged(switchIndex);
    const auto bankOffset = HoldMgr.getPresetAutoRepeatAmplitude(switchIndex, true);
    
    constexpr int NumPresets = 128;
    constexpr int NumBanks = NumPresets / NUM_SCENES;

    int bank = _bank;

    // First handle autorepeat keys:
    if (switchIndex == SWITCH_PRESET_DEC || switchIndex == SWITCH_PRESET_INC) {
      auto newBank = switchIndex == SWITCH_PRESET_DEC ? 
        (bank + NumBanks - bankOffset) % NumBanks :
        (bank + bankOffset) % NumBanks;

      if (fellState || changed) {
        sendProgramChange(newBank * NUM_SCENES + _scene);
        _bank = newBank;
        _leds.flashLed(switchIndex, PEDAL_ACTIVE_FLASH );
        HoldMgr.clearChanged(switchIndex);
      }
    }

    else if (fellState) {
      switch ( switchIndex ) {

        // Switches 1-4, 6-9 (Scene 1-8)
        case SWITCH_S1: case SWITCH_S2: case SWITCH_S3: case SWITCH_S4:
        case SWITCH_S5: case SWITCH_S6: case SWITCH_S7: case SWITCH_S8:
            _scene = sceneFromSwitchValue(switchIndex) - 1;
            sendProgramChange(_bank * NUM_SCENES + _scene);
          break;
      }
      HoldMgr.clearChanged(switchIndex);
    }
  }
}