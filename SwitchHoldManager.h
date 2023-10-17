#pragma once
constexpr long  HoldInterval1 = 1200;
constexpr long  HoldInterval2 = 2400;
constexpr long  HoldInterval3 = 4800;
constexpr long  TickInterval  =  300;

enum SwitchHoldState{
    None,
    Hold1,
    Hold2,
    Hold3
};

struct SwitchHoldManager
{
  SwitchHoldManager() {
    for (int i=0; i < NUM_BUTTONS; i++) {
       HoldStartTimeMs[i]    = 0;
       CurrentState[i] = None;
       Ticks[i] = 0;
       Changed[i] = false;
    }
  }

  void update(int switchIndex, bool fellState, bool roseState, long nowTimeMs) {
    long holdTimeMs;
    auto previousState = CurrentState[switchIndex];

    if (fellState) {
      HoldStartTimeMs[switchIndex] = nowTimeMs;
      holdTimeMs = 0;
      Changed[switchIndex] = true;

    } else if (roseState) {
      HoldStartTimeMs[switchIndex] = 0;
      CurrentState[switchIndex] = SwitchHoldState::None;
      Ticks[switchIndex] = 0;
      Changed[switchIndex] = false;

    } else {
      if (HoldStartTimeMs[switchIndex] > 0)  {
        holdTimeMs = nowTimeMs - HoldStartTimeMs[switchIndex];
        
        bool hold1 = holdTimeMs >= HoldInterval1;
        bool hold2 = holdTimeMs >= HoldInterval2;
        bool hold3 = holdTimeMs >= HoldInterval3;
        if (hold3) {
          CurrentState[switchIndex] = SwitchHoldState::Hold3;
        } else if (hold2) {
              CurrentState[switchIndex] = SwitchHoldState::Hold2;
        } else if (hold1) {
              CurrentState[switchIndex] = SwitchHoldState::Hold1;
        } 

        const auto ticks = holdTimeMs / TickInterval;
        const auto previousTicks = Ticks[switchIndex];
        Ticks[switchIndex] = ticks;

        if (previousTicks != ticks) {
          Changed[switchIndex] = true;
#ifdef DEBUG
          Serial.print(F("Changed "));Serial.println((int) ticks);
#endif
        }
      } // if (HoldStartTimeMs[switchIndex] > 0) 
    } // else
    if  (previousState != CurrentState[switchIndex]) {
      Changed[switchIndex] = true;
#ifdef DEBUG
          Serial.print(F("HOLD "));Serial.println((int) CurrentState[switchIndex]);
#endif
    }
  }

  SwitchHoldState GetHoldState(int switchIndex) const { return CurrentState[switchIndex]; }
  bool hasChanged(int switchIndex) const { return Changed[switchIndex]; }
  void clearChanged(int switchIndex) { Changed[switchIndex] = false; }
  int  getChangeFactor(int switchIndex) const { return (int) CurrentState[switchIndex]; }

  int  getPresetAutoRepeatAmplitude(int switchIndex, bool isStandardMidi=false) const { 
    switch (getChangeFactor(switchIndex)) {
      case 1: return  isStandardMidi ? 2 : 10;
      case 2: return  isStandardMidi ? 4 : 100;
      case 3: return  isStandardMidi ? 8 : 200;
    }
    return 1;
  }

private:
   long HoldStartTimeMs[NUM_BUTTONS];
   long Ticks[NUM_BUTTONS];
   bool Changed[NUM_BUTTONS];
   SwitchHoldState CurrentState[NUM_BUTTONS];
};
