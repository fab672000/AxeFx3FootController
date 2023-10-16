#include "config.h"
#include "FcManager.h"
#include "FcDisplay.h"
#include "FcLeds.h"
#include <Bounce2.h>
#include "ExpPedals.h"
#include "FastMux.h"
#include "TimerUtils.h"

static FastMux mux(2, 3, 4, 5);
static ProtocolType _protType;
static FcDisplay _display;
static FcLeds leds;

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
 
// BUTTONS handling

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

static Bounce Buttons[NUM_BUTTONS];

void FcManager::initButtons() {
  for (byte i = 0; i < NUM_BUTTONS; i++) {
    pinMode(ButtonPin[i], INPUT_PULLUP);

#ifdef HAS_MUX
    mux.select(i);
#endif
    Buttons[i].attach(ButtonPin[i], INPUT_PULLUP);
    Buttons[i].interval(DEFAULT_DEBOUNCE);
  }
}

FcManager::FcManager(ProtocolType protType) {
  _protType = protType;
}

/// Controller initialization
void FcManager::begin() {
  
  leds.begin();
  _display.init();
#ifdef DEBUG
  Serial.begin(115200);
#endif
  initButtons();
  
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
  leds.update(); // update the timer
  Axe.update();
  handleEvents();
 // handleLayoutChange();

}

/// foot switch pressed for a longer time
/// TODO: use the 2 first buttons rows and long timing to switch layouts
/// as an example the tuner + the loop functiona could be remapped on the second row
void FcManager::handleLayoutChange(){
    // long press function to Switch 11
//  if (Buttons[12].pressedFor(2000)) {
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
  Serial.print(F("onPresetChanging(): "));
  Serial.println(number);
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
  leds.turnOnSceneLed(SceNumb);
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

void FcManager::handleEvents() {
  static bool looperPlaying = false;
  static int lastScene = -1;
  static int lastLoopPreset = -1;
  
  int sceneOffset; // +1 if second scene row
  bool isTunerEngaged;
  
  for (byte currentSwitch = 0; currentSwitch < NUM_BUTTONS; currentSwitch++) {
#ifdef HAS_MUX
    mux.select(currentSwitch);
#endif
    Buttons[currentSwitch].update();
    if (Buttons[currentSwitch].fell()) {
      isTunerEngaged = Axe.isTunerEngaged();
      
      if (currentSwitch == 13 || isTunerEngaged ) // tuner switch
      {
            Axe.toggleTuner();
           isTunerEngaged = Axe.isTunerEngaged();
      }
      switch ( currentSwitch ) {

        // Switches 1-4, 6-9 (Scene 1-8)
        case 0: case 1: case 2: case 3:
        case 5: case 6: case 7:case 8:
            sceneOffset = currentSwitch > 3 ? 0 : 1;
            lastScene = currentSwitch + sceneOffset ;
            doSceneChange(lastScene );
            leds.turnOnSceneLed (lastScene );
            leds.setLooperLeds(0);
          break;
          
        // Switch 5  (Preset Decrement)        
        case 4: 
        // Switch 10 (Preset Increment)
        case 9:
          if(currentSwitch == 4 ) {
            Axe.sendPresetDecrement();
            PresetNumb--;
          } else {
            Axe.sendPresetIncrement();
            PresetNumb++;
          }

          leds.flashLed(currentSwitch, PEDAL_ACTIVE_FLASH );
          if(lastLoopPreset>=0)
          {
            leds.setLooperLeds( PresetNumb == lastLoopPreset && looperPlaying ? 2 : 0);
          }
          break;

        // Switch 11 (Looper Record)
        case 10:
            
            Axe.getLooper().record();
            leds.setLooperLeds(1);
            leds.turnOffSceneLeds();
            lastLoopPreset = PresetNumb;
            _display.print(F("RECORD              "));
          break;

        // Switch 12 (Looper Play)
        case 11:
            Axe.getLooper().play();
            if (!looperPlaying) {
              leds.setLooperLeds(2);
              leds.turnOffSceneLeds();
              _display.print(F("PLAY                "));
            } else {
              _display.print(F("STOP                "));
              leds.setLooperLeds(0);
              leds.turnOnSceneLed (lastScene );
              lastLoopPreset = -1;
            }
            looperPlaying = !looperPlaying;
          break;

        // Switch 13 (Looper Undo)
        case 12:
            Axe.getLooper().undo();
            leds.setLooperLeds(3);
            leds.turnOffSceneLeds();
            _display.print(F("UNDO/ERASE          "));
          break;

        default:
          break;

      }

      leds.setTunerLed(isTunerEngaged);
      Serial.println(F("-----------"));
      Serial.print(F("Switch ")); Serial.print(currentSwitch + 1); Serial.println(F(" pressed."));

    } // if Buttons[currentSwitch].fell()) 
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
      
#ifdef DEBUG
      char buffer[32];
      sprintf(buffer, "%6ld:ExpPedal-%d:%d", deltaTime, i, anaValue);
      Serial.println(buffer);
#endif
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
  leds.turnOnSceneLed(scene);
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
  // leds.flashLed( 3, TAP_TEMPO_LED_DURATION ); // pending assign to correspondent tempo led
}
