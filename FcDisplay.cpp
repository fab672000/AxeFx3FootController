#include <Arduino.h>
#include "FcDisplay.h"
#include "TimerUtils.h"

// According to AxeEdit III, only 31 chararacters max can be entered
static char bufPresetName[MaxPresetNameLen + 1];
static char bufSceneName[MaxSceneNameLen + 1];

#if (DISPLAY_TYPE == LIQUID_DISPLAY_I2C)
#include <LiquidCrystal_I2C.h>
    /* LCD definition
       lcd(RS, Enable, D4, D5, D6, D7, BL)
       KEY pin to analogl pin 0
    */
    //LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
    // Set the LCD address to 0x27 for a 20 chars and 4 line display
static LiquidCrystal_I2C lcd(0x27, 20, 4);

#elif (DISPLAY_TYPE == ADAFRUIT_LCD_SHIELD)
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

static Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#endif

/// Initializes the display device selected in config.h
void FcDisplay::init() {
#if (DISPLAY_TYPE == LIQUID_DISPLAY_I2C)
    lcd.begin();
    lcd.backlight();
    
#elif (DISPLAY_TYPE == ADAFRUIT_LCD_SHIELD)
    lcd.begin(16, 2);
    lcd.setBacklight(TEAL);
#endif
#if (DISPLAY_TYPE != NO_DISPLAY)
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("AxeFX3 EcoFootCtrl"));
    lcd.setCursor(0, 1);
    lcd.print(F("Waiting connection.."));
#endif
}

/// Clear the display screen
void FcDisplay::clear() const { 
#if (DISPLAY_TYPE != NO_DISPLAY)
  lcd.clear(); 
#endif
}

char*  FcDisplay::presetNameString() { return bufPresetName;}
char*  FcDisplay::sceneNameString()  { return bufSceneName;}

///
void  FcDisplay::presetNameToLCD(int presetNum, byte sceneNum) const {
#if (DISPLAY_TYPE != NO_DISPLAY)
  // Preset Name to LCD display
  int lineCount = 0;
  int posY = 0;
  int posX = 0;
  int maxCharacters = 20;
  byte stillProcessing = 1;
  byte charCount = 1;

  lcd.clear();
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
  lcd.setCursor(17, 1); lcd.print(presetNum);

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
  lcd.setCursor(17, 3); lcd.print(sceneNum);
#endif 
}

void FcDisplay::displayFineTune(int fineTune) const {
   //char buf[50];
  //snprintf(buf, 50, "TUNING: note=%s string=%d fineTune=%d", note, string, fineTune);
  //Serial.println(buf);
#if (DISPLAY_TYPE != NO_DISPLAY)

  lcd.setCursor(0, 0);
  lcd.print(F("                    "));
  //lcd.setCursor(9, 0);
  // lcd.print(note);
  // lcd.print(string);
  //lcd.print("   ");

  lcd.setCursor(0, 1);
  if ( fineTune <= 61)                       {    lcd.print(F("                   "));}
  if ((fineTune >= 62) && (fineTune <= 64))  {    lcd.print(F("----->>>><<<<------"));}
  if ( fineTune >= 65)                       {    lcd.print(F("                  +"));}
  
  lcd.setCursor(0, 2);  
  if (fineTune <= 15)                        {    lcd.print(F(">                   "));}
  if ((fineTune >= 16) && (fineTune <= 23))  {    lcd.print(F(">>                  "));}
  if ((fineTune >= 24) && (fineTune <= 28))  {    lcd.print(F("->>                 "));}
  if ((fineTune >= 29) && (fineTune <= 34))  {    lcd.print(F("-->>                "));}
  if ((fineTune >= 35) && (fineTune <= 39))  {    lcd.print(F("--->>               "));}
  if ((fineTune >= 40) && (fineTune <= 45))  {    lcd.print(F("---->>              "));}
  if ((fineTune >= 46) && (fineTune <= 50))  {    lcd.print(F("----->>             "));}
  if ((fineTune >= 51) && (fineTune <= 56))  {    lcd.print(F("------>>            "));}
  if ((fineTune >= 57) && (fineTune <= 61))  {    lcd.print(F("------->>           "));}
  if ((fineTune >= 62) && (fineTune <= 64))  {    lcd.print(F("------>>>><<<<------"));}
  if ((fineTune >= 65) && (fineTune <= 70))  {    lcd.print(F("           <<-------"));}
  if ((fineTune >= 71) && (fineTune <= 75))  {    lcd.print(F("            <<------"));}
  if ((fineTune >= 76) && (fineTune <= 81))  {    lcd.print(F("             <<-----"));}
  if ((fineTune >= 82) && (fineTune <= 86))  {    lcd.print(F("              <<----"));}
  if ((fineTune >= 87) && (fineTune <= 92))  {    lcd.print(F("               <<---"));}
  if ((fineTune >= 93) && (fineTune <= 98))  {    lcd.print(F("                <<--"));}
  if ((fineTune >= 99) && (fineTune <= 103)) {    lcd.print(F("                 <<-"));}
  if ((fineTune >= 104) && (fineTune <= 110)){    lcd.print(F("                  <<"));}
  if (fineTune >= 101)                       {    lcd.print(F("                   <"));}

  lcd.setCursor(0, 3);
  if ( fineTune <= 61)                       {    lcd.print(F("-                   "));}
  if ((fineTune >= 62) && (fineTune <= 64))  {    lcd.print(F("------>>>><<<<------"));}
  if ( fineTune >= 65)                       {    lcd.print(F("                   +"));}
#endif
}

void FcDisplay::displayControllerValue(const char* label, byte num, byte value) const {
#if (DISPLAY_TYPE != NO_DISPLAY)
  char buff[16];
  sprintf(buff, "%s-%d %03d", label, num, value); // buff will be "01238"
  lcd.setCursor(0, 1); 
  lcd.print(buff); 
#endif
}

void FcDisplay::clearControllerValue(const char* label, byte num) const {
#if (DISPLAY_TYPE != NO_DISPLAY) 
  lcd.setCursor(0, 1); 
  lcd.print(F("            "));
#endif  
}

void FcDisplay::print(const __FlashStringHelper* text) const {
#if (DISPLAY_TYPE != NO_DISPLAY)
//lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print(text);
#endif
}
