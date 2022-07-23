#pragma once
#include "lcd.h"

void onTunerData(const char *note, const byte string, const byte fineTune) {
  //char buf[50];
  //snprintf(buf, 50, "TUNING: note=%s string=%d fineTune=%d", note, string, fineTune);
  //Serial.println(buf);

  lcd.setCursor(0, 0);
  lcd.print("                    ");
  //lcd.setCursor(9, 0);
  // lcd.print(note);
  // lcd.print(string);
  //lcd.print("   ");

  lcd.setCursor(0, 1);
  if ( fineTune <= 61)                       {    lcd.print("-                   ");}
  if ((fineTune >= 62) && (fineTune <= 64))  {    lcd.print("------>>>><<<<------");}
  if ( fineTune >= 65)                       {    lcd.print("                   +");}
  
  lcd.setCursor(0, 2);  
  if (fineTune <= 15)                        {    lcd.print(">                   ");}
  if ((fineTune >= 16) && (fineTune <= 23))  {    lcd.print(">>                  ");}
  if ((fineTune >= 24) && (fineTune <= 28))  {    lcd.print("->>                 ");}
  if ((fineTune >= 29) && (fineTune <= 34))  {    lcd.print("-->>                ");}
  if ((fineTune >= 35) && (fineTune <= 39))  {    lcd.print("--->>               ");}
  if ((fineTune >= 40) && (fineTune <= 45))  {    lcd.print("---->>              ");}
  if ((fineTune >= 46) && (fineTune <= 50))  {    lcd.print("----->>             ");}
  if ((fineTune >= 51) && (fineTune <= 56))  {    lcd.print("------>>            ");}
  if ((fineTune >= 57) && (fineTune <= 61))  {    lcd.print("------->>           ");}
  if ((fineTune >= 62) && (fineTune <= 64))  {    lcd.print("------>>>><<<<------");}
  if ((fineTune >= 65) && (fineTune <= 70))  {    lcd.print("           <<-------");}
  if ((fineTune >= 71) && (fineTune <= 75))  {    lcd.print("            <<------");}
  if ((fineTune >= 76) && (fineTune <= 81))  {    lcd.print("             <<-----");}
  if ((fineTune >= 82) && (fineTune <= 86))  {    lcd.print("              <<----");}
  if ((fineTune >= 87) && (fineTune <= 92))  {    lcd.print("               <<---");}
  if ((fineTune >= 93) && (fineTune <= 98))  {    lcd.print("                <<--");}
  if ((fineTune >= 99) && (fineTune <= 103)) {    lcd.print("                 <<-");}
  if ((fineTune >= 104) && (fineTune <= 110)){    lcd.print("                  <<");}
  if (fineTune >= 101)                       {    lcd.print("                   <");}

  lcd.setCursor(0, 3);
  if ( fineTune <= 61)                       {    lcd.print("-                   ");}
  if ((fineTune >= 62) && (fineTune <= 64))  {    lcd.print("------>>>><<<<------");}
  if ( fineTune >= 65)                       {    lcd.print("                   +");}

}

void onTunerStatus(bool engaged) {
  if (Axe.isTunerEngaged() == false)
  {
    Axe.update();
    lcd.clear();
    presetNameToLCD();
  }
}
