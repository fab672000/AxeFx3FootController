#include "config.h"
#include "FcManager.h"
#include "ExpPedals.h"
#include "FastMux.h"
#include "TimerUtils.h"
#include "SwitchHoldManager.h"

static AxeSystem Axe;


//TODO: investigate for use of more tempo  functionality
// static Tempo tempo;

// Pedal / controller definitions
static unsigned int pedalValue[numberOfPedals];
static unsigned int pedalValueOld[numberOfPedals];
static byte controllerValueOld[numberOfPedals];
static byte controllerValue[numberOfPedals];

// SCENE / PRESETS handling
#define NUM_SCENES 8

//Struct to hold information about each scenePlay
struct SceneInfo {
  SceneNumber number = -1;
  const char *name;
};


//A list of all of the scenes for current preset
static SceneInfo scenes[NUM_SCENES];

static int  PresetNumb;
static byte SceNumb;
 
FcManager::FcManager() : FootController(MIDI_AFX3)
{

}

/// Controller initialization
void FcManager::init() {
  
  Axe.begin(MIDI_PORT); // TODO in future: implement more midi devices not necesarily assuming a direct serial connection
  Axe.registerPresetChangeCallback(onPresetChange);
  Axe.registerPresetChangingCallback(onPresetChanging);
  Axe.registerSystemChangeCallback(onSystemChange);
  Axe.registerSceneNameCallback(onSceneName);
  Axe.registerTunerStatusCallback(onTunerStatus);
  Axe.registerTunerDataCallback(onTunerData);
  Axe.registerTapTempoCallback(onTapTempo);
  Axe.enableRefresh(AXE_REFRESH_RATE);
  //Axe.requestPresetDetails();
  Axe.refresh(true);
}

/// Controller update handling
void FcManager::update() {
  _leds.update(); // update the timer
  Axe.update();
  handleEvents();
 // handleLayoutChange();

}

/// foot switch pressed for a longer time
/// TODO: use the 2 first buttons rows and long timing to switch layouts
/// as an example the tuner + the loop functiona could be remapped on the second row
void FcManager::handleLayoutChange(){
    // long press function to Switch 11
//  if (_buttons[12].pressedFor(2000)) {
//    PL_("Switch long pressed");
//  }
}

void FcManager::notifyPresetChanged(AxePreset preset) {
  if (!Axe.isTunerEngaged() )
  {
   int num = preset.getPresetNumber();
   Axe.sendPresetChange(num);
  }
}

void FcManager::onSceneName(const SceneNumber number, const char* name, const byte length) {
  if (strcmp(scenes[number-1].name,  name)==0) return;
 
  Serial.print(F("onSceneName(): "));
  Serial.print(number);
  Serial.print(F(": "));
  Serial.println(name);

  //Record current scene in the list  scenes[number-1].number = number;
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

// Fetch All Scene Names
void FcManager::onPresetChanging(const PresetNumber number) {
  //Reset the scene list for the new preset
  for (byte i=0; i<NUM_SCENES; i++) {
    scenes[i].number = -1;
  }

  if(number==PresetNumb) return;
#ifdef DEBUG
  if (number>=0) {
    Serial.print(F("onPresetChanging(): "));
    Serial.println(number);
  }
#endif  
  }

void FcManager::onPresetChange(AxePreset preset) {
  PresetNumb = preset.getPresetNumber();
  preset.copyPresetName(FcDisplay::presetNameString(), MaxPresetNameLen + 1);
  Serial.print(F("onPresetChange(): "));
  Serial.print(PresetNumb);
  Serial.print(F(": "));
  
  Serial.println(preset.getPresetName());

  SceNumb = preset.getSceneNumber();
  preset.copySceneName(FcDisplay::sceneNameString(), MaxSceneNameLen + 1);
  _display.presetNameToLCD(PresetNumb, SceNumb);
  _leds.turnOnSceneLed(SceNumb);
}

void FcManager::onTunerData(const char *note, const byte string, const byte fineTune) {
  _display.displayFineTune(fineTune);
}

void FcManager::onTunerStatus(bool engaged) {
  if (!Axe.isTunerEngaged())
  {
    _display.clear();
    _display.presetNameToLCD(PresetNumb, SceNumb);
  }
}

int FcManager::sceneFromSwitchValue(int sw) {
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


static SwitchHoldManager HoldMgr;

void FcManager::handleEvents() {
  static bool looperPlaying = false;
  static int lastScene = -1;
  static int lastLoopPreset = -1;
  
  int scene; // +1 if second scene row
  bool isTunerEngaged;
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
    const auto presetOffset = HoldMgr.getPresetAutoRepeatAmplitude(switchIndex);
    
    // First handle autorepeat keys:
    if (switchIndex == SWITCH_PRESET_DEC || switchIndex == SWITCH_PRESET_INC) {
      auto newPreset = switchIndex == SWITCH_PRESET_DEC ? 
        (PresetNumb + 1024 - presetOffset) % 1024 :
        (PresetNumb + presetOffset) % 1024;

      if (fellState || changed) {
        if (switchIndex == SWITCH_PRESET_DEC ) {
          if (presetOffset == 1)  Axe.sendPresetDecrement(); else Axe.sendPresetChange(newPreset);
        } else {
          if (presetOffset == 1)  Axe.sendPresetIncrement(); else Axe.sendPresetChange(newPreset);
        }
        PresetNumb = newPreset;

        _leds.flashLed(switchIndex, PEDAL_ACTIVE_FLASH );
        if(lastLoopPreset >= 0) {
          _leds.setLooperLeds( PresetNumb == lastLoopPreset && looperPlaying ? 2 : 0);
        }

        HoldMgr.clearChanged(switchIndex);
      }
    }

    else if (fellState) {

      isTunerEngaged = Axe.isTunerEngaged();
      
      if (switchIndex == SWITCH_TUNER || isTunerEngaged ) {
            Axe.toggleTuner();
           isTunerEngaged = Axe.isTunerEngaged();
      }
      switch ( switchIndex ) {

        // Switches 1-4, 6-9 (Scene 1-8)
        case SWITCH_S1: case SWITCH_S2: case SWITCH_S3: case SWITCH_S4:
        case SWITCH_S5: case SWITCH_S6: case SWITCH_S7: case SWITCH_S8:
            scene = sceneFromSwitchValue(switchIndex);
            doSceneChange(scene );
            _leds.turnOnSceneLed (scene );
            lastScene = scene;
            _leds.setLooperLeds(0);
          break;
          
        case SWITCH_LOOPER_RECORD:
            
            Axe.getLooper().record();
            _leds.setLooperLeds(1);
            _leds.turnOffSceneLeds();
            lastLoopPreset = PresetNumb;
            _display.print(F("RECORD              "));
          break;

        case SWITCH_LOOPER_PLAY:
            Axe.getLooper().play();
            if (!looperPlaying) {
              _leds.setLooperLeds(2);
              _leds.turnOffSceneLeds();
              _display.print(F("PLAY                "));
            } else {
              _display.print(F("STOP                "));
              _leds.setLooperLeds(0);
              _leds.turnOnSceneLed (lastScene );
              lastLoopPreset = -1;
            }
            looperPlaying = !looperPlaying;
          break;

        case SWITCH_LOOPER_UNDO:
            Axe.getLooper().undo();
            _leds.setLooperLeds(3);
            _leds.turnOffSceneLeds();
            _display.print(F("UNDO/ERASE          "));
          break;

        case SWITCH_TAP_TEMPO: // tap tempo
          Axe.sendTap();
          break;
      }

      _leds.setTunerLed(isTunerEngaged);
      Serial.println(F("-----------"));
      Serial.print(F("Switch ")); Serial.print(switchIndex + 1); Serial.println(F(" pressed."));

    } // if currentButton.fell()) 
  } // for
  
  // TODO:
  handleExpressionPedals();
}

void FcManager::handleExpressionPedals() {
  static unsigned long lastTimeActive=0UL;
  static bool mustClear = false;
  unsigned long currentTime = millis();

  //This line smooths the measured values. The measured values of an unused potentiometer can jumb back and forth between values.
  // Without the smoothing the controller would constantly send CC data, which I don't want. There are other ways to smooth, but this one works best for me.
  // The values 0.4 and 0.6 need to add up to 1.0. 0.4 and 0.6 gave the best results. Change to taste!
  for (int i = 0; i < numberOfPedals; i++) {
    pedalValue[i] = analogRead(pedal[i]) * 0.4 + pedalValueOld[i] * 0.6;

    //Here the measured values are scaled down to 0 to 127
    controllerValue[i] = map(pedalValue[i], pedalValueCalibrationMinimumLevel[i],
                                        pedalValueCalibrationMaximumLevel[i], controllerValueMinimumLevel[i], controllerValueMaximumLevel[i]);

    //Only send CC data in case the potentiometer / expression pedal is being used (turned) and in case it's not deactivated
    if (controllerValue[i]!=0 && controllerValue[i] != controllerValueOld[i]) {

      Axe.sendControlChange(pedalCC[i], controllerValue[i], MidiChannel);
      _display.displayControllerValue("Exp", i+1, controllerValue[i]);
      lastTimeActive = currentTime;
      mustClear = true;
    } else if (mustClear && abs(currentTime-lastTimeActive)>AUTO_CLEAR_CONTROL_MS) {
        _display.clearControllerValue("Exp", 0);
        mustClear = false;
    }
    controllerValueOld[i] = controllerValue[i];
    pedalValueOld[i] = pedalValue[i];
  }
}

void FcManager::doSceneChange(byte scene) {
  _leds.turnOnSceneLed(scene);
  Axe.sendSceneChange(scene);
  Axe.update();
}

// =======================================================================================================
// TODO: future


void FcManager::onSystemChange() {
  // Display the current tempo at the LCD
  // lcd.setCursor(0, 3); lcd.print(F("Tempo: ")); lcd.print(Axe.getTempo()); lcd.print(F("  "));
}

//this will only work if realtime sysex is enabled
void FcManager::onTapTempo() {
  // Flashes a LED on tempo
  // _leds.flashLed( 3, TAP_TEMPO_LED_DURATION ); // pending assign to correspondent tempo led
}
