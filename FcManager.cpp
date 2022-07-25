#include "config.h"
#include "FcManager.h"
#include "FcDisplay.h"
#include "FcLeds.h"
#include "SC_Button.h"
#include "ExpPedals.h"


static ProtocolType _protType;
static FcDisplay _display;

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

//Struct to hold information about each scene
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
#if (BOARD == BOARD_ATMEGA) 
  BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7, BUTTON8,
  BUTTON9, BUTTON10, BUTTON11, BUTTON12, BUTTON13, BUTTON14, BUTTON15, BUTTON16
#elif (BOARD == BOARD_MINI_TESTING) 
  BUTTON1, BUTTON2, BUTTON3, BUTTON4
#endif
};

static Button Buttons[NUM_BUTTONS];

void FcManager::initButtons() {
  for (byte i = 0; i < NUM_BUTTONS; i++) {
    Buttons[i].setPin(ButtonPin[i]);
    Buttons[i].setDebounceTime(DEFAULT_DEBOUNCE);
    Buttons[i].setPullupEnable(true);
    Buttons[i].begin();
  }
}

FcManager::FcManager(ProtocolType protType) {
  _protType = protType;
}

/// Controller initialization
void FcManager::begin() {
  _display.init();
  initButtons();
  FcLeds::begin();
  
  Axe.begin(MIDI_PORT); // TODO in future: implement more midi devices not necesarily assuming a direct serial connection
  Axe.registerPresetChangeCallback(onPresetChange);
  // Axe.registerPresetChangingCallback(onPresetChanging);
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
  FcLeds::update(); // update the timer
  Axe.update();
  handleEvents();
 // handleLayoutChange();

}

/// foot switch pressed for a longer time
/// TODO: use the 2 first buttons rows and long timing to switch layouts
/// as an example the tuner + the loop functiona could be remapped on the second row
void FcManager::handleLayoutChange(){
    // long press function to Switch 11
  if (Buttons[12].pressedFor(2000)) {
    PL_("Switch long pressed");
  }
}

void FcManager::notifyPresetChanged(AxePreset preset) {
  if (!Axe.isTunerEngaged() )
  {
   int num = preset.getPresetNumber();
   Axe.sendPresetChange(num);
  }
}

void FcManager::onPresetChange(AxePreset preset) {
  PresetNumb = preset.getPresetNumber();
  preset.copyPresetName(FcDisplay::presetNameString(), MaxPresetNameLen + 1);

  SceNumb = preset.getSceneNumber();
  preset.copySceneName(FcDisplay::sceneNameString(), MaxSceneNameLen + 1);
  _display.presetNameToLCD(PresetNumb, SceNumb);
}

void FcManager::onTunerData(const char *note, const byte string, const byte fineTune) {
  _display.displayFineTune(fineTune);
}

void FcManager::onTunerStatus(bool engaged) {
  if (!Axe.isTunerEngaged())
  {
    Axe.update();
    _display.clear();
    _display.presetNameToLCD(PresetNumb, SceNumb);
  }
}

void FcManager::onSceneName(const SceneNumber number, const char* name, const byte length) {
  if (number == 1) {
    Serial.println("\n+++++++++++**+++++++++++\n\n");
  }

  Serial.print("onSceneName(): ");
  Serial.print(number);
  Serial.print(": ");
  Serial.println(name);

  //Record current scene in the list
  scenes[number-1].number = number;
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

void FcManager::handleEvents() {
  for (byte currentSwitch = 0; currentSwitch < NUM_BUTTONS; currentSwitch++) {

    Buttons[currentSwitch].read();
    if (Buttons[currentSwitch].wasPressed()) {

      switch ( currentSwitch ) {

        // Switch 1-4 (Scene 1-4)
        case 0:
        case 1:
        case 2:
        case 3:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else
          {
            doSceneChange(currentSwitch + 1);
            FcLeds::turnOnSceneLed (currentSwitch + 1);
          }
          break;

        // Switch 5 (Preset Decrement)
        case 4:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.sendPresetDecrement();
            FcLeds::flashLed(currentSwitch, PEDAL_ACTIVE_FLASH );
          }
          break;

        // Switch 6-9 (Scene 5-9)
        case 5:
        case 6:
        case 7:
        case 8:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            doSceneChange(currentSwitch);
            FcLeds::turnOnSceneLed (currentSwitch + 1);
          }
          break;

        // Switch 10 (Preset Increment)
        case 9:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.sendPresetIncrement();
            FcLeds::flashLed( currentSwitch, PEDAL_ACTIVE_FLASH );
          }
          break;

        // Switch 11 (Looper Record)
        case 10:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.getLooper().record();
            FcLeds::setLooperLeds(1);
            //FcLeds::turnOffSceneLeds();
            _display.print("RECORD              ");


            //  uint32_t ti = (Buttons[currentSwitch].pressedFor());
            //  PL_(ti);

          }
          break;

        // Switch 12 (Looper Play)
        case 11:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.getLooper().play();
            FcLeds::setLooperLeds(2);
            _display.print("PLAY                ");
          }
          break;

        // Switch 13 (Looper Undo)
        case 12:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.getLooper().undo();
            FcLeds::setLooperLeds(3);
            FcLeds::turnOffSceneLeds();
            _display.print("UNDO                ");
          }
          break;

        // Switch 14 - Swapped to 15 (TUNER ENGAGE)
        case 13:
          Serial.println("Switch-15 (14) pressed");

          Axe.toggleTuner();
          FcLeds::setTunerLed(Axe.isTunerEngaged());

          break;

        // Switch 15 - Swapped to 14
        case 14:



          break;

        // Switch 16.
        case 15:
          //Serial.println("Switch-16 pressed");

          break;

      }
      Serial.println("-----------");
      Serial.print("Switch "); Serial.print(currentSwitch + 1); Serial.println(" pressed.");
    }
  }
  
  // TODO:
  // handleExpressionPedals();
}

void FcManager::handleExpressionPedals() {
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
    Serial.print("ExpPedal-1: "); 
    Serial.println(controllerValue[i]);
#endif

      _display.displayControllerValue("ExpPedal-1", controllerValue[i]);
    }
    controllerValueOld[i] = controllerValue[i];
    pedalValueOld[i] = pedalValue[i];
  }
}

void FcManager::doSceneChange(byte scene) {
  FcLeds::turnOnSceneLed(scene);
  Axe.sendSceneChange(scene);
  Axe.update();
}

// Fetch All Scene Names
void FcManager::onPresetChanging(const PresetNumber number) {
  //Reset the scene list for the new preset
  for (byte i=0; i<NUM_SCENES; i++) {
    scenes[i].number = -1;
  }
}

// =======================================================================================================
// TODO: future


void FcManager::onSystemChange() {
  // Display the current tempo at the LCD
  // lcd.setCursor(0, 3); lcd.print("Tempo: "); lcd.print(Axe.getTempo()); lcd.print("  ");
}

//this will only work if realtime sysex is enabled
void FcManager::onTapTempo() {
  // Flashes a LED on tempo
  // FcLeds::flashLed( 3, TAP_TEMPO_LED_DURATION ); // pending assign to correspondent tempo led
}
