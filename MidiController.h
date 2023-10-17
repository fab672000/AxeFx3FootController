#pragma once
#include "FootController.h"
#include <MIDI.h>

class MidiController : public FootController
{
  public:
    MidiController();

    void init() override;
    void update()  override;

    byte getBank() const { return _bank; }
    byte getProgramChange() const { return _scene + _bank * NUM_SCENES; }
    void sendProgramChange(byte program);
    void sendControlChange(byte control, byte value);
    void sendBankChange(byte bank);
    void handleEvents();
protected:
    midi::SerialMIDI<HardwareSerial>& serialMIDI;
    midi::MidiInterface<midi::SerialMIDI<HardwareSerial>>& MIDI;
    byte _bank;
    byte _scene;
};