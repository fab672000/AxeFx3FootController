#pragma once
#include "config.h"

const size_t MaxPresetNameLen = 31;
const size_t MaxSceneNameLen = 31;

/**
 * Display utility class
 * The purpose of this class if to ecapsulate all specific code of various display hardware 
 * and propose an opaque foot controller display lib instead.
 */

class FcDisplay
{
public:
  void init();
  void clear() const;
  void presetNameToLCD(int presetNum, byte sceneNum) const;
  void displayFineTune(int fineTune) const;
  void displayControllerValue(const char* label, byte value) const;
  void print(const char* msg) const;

  static char*  presetNameString();
  static char*  sceneNameString();
};
