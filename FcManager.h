#pragma once
#include <AxeFxControl.h>
#include "ProtocolType.h"
#include "FootController.h"

class AxePreset;

//commands
enum Command {
  PRESET_DOWN,
  PRESET_UP,
  SCENE_DOWN,
  SCENE_UP,
  SCENE_CHANGE,
  EFFECT_TOGGLE,
  MIDI_PC,
  MIDI_CC,
  __COMMAND_COUNT
};

/** Simple Foot controller Manager class for fractal products
 *    In future we could derive it from a controller base class for a complete controller agnostic lib and device!
 */ 
class FcManager : public FootController {
public:
  FcManager();
  
  void init()   override;
  void update()  override;

  void doSceneChange(byte scene);
  void handleExpressionPedals();

private:
  void initButtons();
  void handleEvents();
  void handleLayoutChange();

  void notifyPresetChanged(AxePreset preset);

  static void onPresetChange(AxePreset preset);
  static void onTunerData(const char *note, const byte string, const byte fineTune);
  static void onTunerStatus(bool engaged);
  static void onSceneName(const SceneNumber number, const char* name, const byte length);
  static void onPresetChanging(const PresetNumber number);

  
  // TODO:
  static void onSystemChange();
  static void onTapTempo();
};
