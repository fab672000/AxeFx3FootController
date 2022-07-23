#pragma once
#include "Functions.h"

void initLcd() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AxeFX III Controller");
  lcd.setCursor(0, 1);
  lcd.print("Waiting connection..");
  delay(500);
}


void  presetNameToLCD() {
  lcd.clear();
  // Preset Name to LCD display
  int lineCount = 0;
  int posY = 0;
  int posX = 0;
  int maxCharacters = 20;
  byte stillProcessing = 1;
  byte charCount = 1;
  while (stillProcessing) {
    if (++lineCount > maxCharacters) {    // have we printed maxCharacters yet (+1 for the logic)
      posY += 1;
      lcd.setCursor(posX, posY);  // move cursor down
      lineCount = 1;
      maxCharacters = 20;  // MAximum number of characters on second line
    }
    lcd.print(bufPresetName[charCount - 1]);

    if (!bufPresetName[charCount]) {   // no more chars to process?
      stillProcessing = 0;
    }
    charCount += 1;
  }

  lcd.setCursor(15, 1); lcd.print("P");
  lcd.setCursor(17, 1); lcd.print(PresetNumb);

  // Scene Name to LCD display
  lineCount = 0;
  posY = 2;
  posX = 0;
  maxCharacters = 20;
  stillProcessing = 1;
  charCount = 1;
  lcd.setCursor(posX, posY);
  while (stillProcessing) {
    if (++lineCount > maxCharacters) {    // have we printed maxCharacters yet (+1 for the logic)
      posY += 1;
      lcd.setCursor(posX, posY);  // move cursor down
      lineCount = 1;
      maxCharacters = 20;  // MAximum number of characters on second line
    }
    lcd.print(bufSceneName[charCount - 1]);

    if (!bufSceneName[charCount]) {   // no more chars to process?
      stillProcessing = 0;
    }
    charCount += 1;
  }

  lcd.setCursor(15, 3); lcd.print("S");
  lcd.setCursor(17, 3); lcd.print(SceNumb);
}
