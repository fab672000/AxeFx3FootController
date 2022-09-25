#pragma once
#include "config.h"
#include <Arduino.h>

// Thanks to axifist for the original code
// https://forum.fractalaudio.com/threads/diy-axe-fx-foot-controller-for-50-open-project.119882/page-5#post-1705256

const byte numberOfPedals = 2;// number of pedals to be used
#if (BOARD == BOARD_SW16_EXP4) 
const byte MAX_PEDALS = 4;// max number of pedals to be used
const byte PEDAL01 =  A0;
const byte PEDAL02 =  A1;

#ifdef TEENSY_BOARD
const byte PEDAL03 =  A2;
const byte PEDAL04 =  A3;
#else
const byte PEDAL03 = A10;
const byte PEDAL04 = A11;
#endif

//const byte PEDAL05 = A12;
//const byte PEDAL06 = A13;
//const byte PEDAL07 = A14;
//const byte PEDAL08 = A15;

const byte pedal[MAX_PEDALS] PROGMEM = { PEDAL01, PEDAL02, PEDAL03, PEDAL04
// , PEDAL05, PEDAL06, PEDAL07, PEDAL08
};
const unsigned int pedalValueCalibrationMinimumLevel[] PROGMEM = {     0,      0,      0,      0};
const unsigned int pedalValueCalibrationMaximumLevel[] PROGMEM = {  1021,   1021,   1021,   1021};
const byte controllerValueMinimumLevel[]               PROGMEM = {     0,      0,      0,      0};
const byte controllerValueMaximumLevel[]               PROGMEM = {   127,    127,    127,    127};
const byte pedalCC[]                                   PROGMEM = {    16,      10,     18,     19}; //define the CC for each pedal

//const unsigned int pedalValueCalibrationMinimumLevel[] PROGMEM = {     0,      0,      0,      0,      0,      0,      0,      0};
//const unsigned int pedalValueCalibrationMaximumLevel[] PROGMEM = {  1021,   1021,   1021,   1021,   1021,   1021,   1021,   1021};
//const byte controllerValueMinimumLevel[]               PROGMEM = {     0,      0,      0,      0,      0,      0,      0,      0};
//const byte controllerValueMaximumLevel[]               PROGMEM = {   127,    127,    127,    127,    127,    127,    127,    127};
//const byte pedalCC[]                                   PROGMEM = {    27,      7,     18,     19,     20,     21,     22,     23}; //define the CC for each pedal
#elif  (BOARD == BOARD_MINI_TESTING) 
const byte MAX_PEDALS = 1;// max number of pedals to be used
const byte PEDAL01 = A0;
const byte pedal[MAX_PEDALS] PROGMEM = { PEDAL01};
const unsigned int pedalValueCalibrationMinimumLevel[] PROGMEM = {     0};
const unsigned int pedalValueCalibrationMaximumLevel[] PROGMEM = {  1021};
const byte controllerValueMinimumLevel[]               PROGMEM = {     0};
const byte controllerValueMaximumLevel[]               PROGMEM = {   127};
const byte pedalCC[]                                   PROGMEM = {    27}; //define the CC for each pedal
#endif
