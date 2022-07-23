#pragma once
#include <Arduino.h>

// Thanks to axifist for the original code
// https://forum.fractalaudio.com/threads/diy-axe-fx-foot-controller-for-50-open-project.119882/page-5#post-1705256

const byte numberOfPedals = 1;// number of pedals to be used
const byte PEDAL01 =  A0;
const byte PEDAL02 =  A1;
const byte PEDAL03 = A10;
const byte PEDAL04 = A11;
const byte PEDAL05 = A12;
const byte PEDAL06 = A13;
const byte PEDAL07 = A14;
const byte PEDAL08 = A15;

const byte pedal[8]                                     = {PEDAL01, PEDAL02, PEDAL03, PEDAL04, PEDAL05, PEDAL06, PEDAL07, PEDAL08};
const unsigned int pedalValueCalibrationMinimumLevel[8] = {     0,      0,      0,      0,      0,      0,      0,      0};
const unsigned int pedalValueCalibrationMaximumLevel[8] = {  1021,   1021,   1021,   1021,   1021,   1021,   1021,   1021};
byte controllerValueMinimumLevel[8]                     = {     0,      0,      0,      0,      0,      0,      0,      0};
byte controllerValueMaximumLevel[8]                     = {   127,    127,    127,    127,    127,    127,    127,    127};
const byte pedalCC[8]                                   = {    27,      7,     18,     19,     20,     21,     22,     23}; //define the CC for each pedal
byte circlePedals = 0;
unsigned int pedalValue[numberOfPedals];
unsigned int pedalValueOld[numberOfPedals];
byte controllerValueOld[numberOfPedals];
byte controllerValue[numberOfPedals];

byte pedalFunction = 1;

void expressionPedals() {

  //This line smooths the measured values. The measured values of an unused potentiometer can jumb back and forth between values.
  // Without the smoothing the controller would constantly send CC data, which I don't want. There are other ways to smooth, but this one works best for me.
  // The values 0.4 and 0.6 need to add up to 1.0. 0.4 and 0.6 gave the best results. Change to taste!
  for (circlePedals = 0; circlePedals < numberOfPedals; circlePedals++) {
    pedalValue[circlePedals] = analogRead(pedal[circlePedals]) * 0.4 + pedalValueOld[circlePedals] * 0.6;

    //Here the measured values are scaled down to 0 to 127
    controllerValue[circlePedals] = map(pedalValue[circlePedals], pedalValueCalibrationMinimumLevel[circlePedals],
                                        pedalValueCalibrationMaximumLevel[circlePedals], controllerValueMinimumLevel[circlePedals], controllerValueMaximumLevel[circlePedals]);

    //Only send CC data in case the potentiometer / expression pedal is being used (turned) and in case it's not deactivated
    if (controllerValue[circlePedals] != controllerValueOld[circlePedals] != 0) {

      Axe.sendControlChange(pedalCC[circlePedals], controllerValue[circlePedals], MidiChannel);
      Serial.print("ExpPedal-1: "); Serial.println(controllerValue[circlePedals]);
      lcd.setCursor(12, 1); lcd.print("   ");
      lcd.setCursor(0, 1); lcd.print("ExpPedal-1: "); lcd.print(controllerValue[circlePedals]);
    }
    controllerValueOld[circlePedals] = controllerValue[circlePedals];
    pedalValueOld[circlePedals] = pedalValue[circlePedals];
  }
}
