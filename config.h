#pragma once
#include <Arduino.h>

// DEBUG Mode:
#define DEBUG

// BOARDS
#define BOARD_ATMEGA  1 /* AVR ATMEGA */
#define BOARD_UNO     2 /* AVR UNO */
#define BOARD_T40    11 /* TEENSY 4.0 */

#define BOARD BOARD_UNO

// DISPLAYS
// TODO: add more displayys to encapsulate in FcDisplay class
#define NO_DISPLAY         0
#define LIQUID_DISPLAY_I2C 1
#define ADAFRUIT_LCD_SHIELD 2

/// #define DISPLAY_TYPE LIQUID_DISPLAY_I2C
#define DISPLAY_TYPE ADAFRUIT_LCD_SHIELD

// MIDI
const byte MidiChannel = 1;

#define P_ Serial.print
#define PL_ Serial.println

#define AXE_REFRESH_RATE 1500

#define PEDAL_ACTIVE_FLASH 50 // Delay for flash when pedal is pressed
#define TAP_TEMPO_LED_DURATION 100
#define DEFAULT_DEBOUNCE 80

#if (BOARD == BOARD_ATMEGA) 
#define MIDI_PORT Serial1

#define NUM_BUTTONS 16
#define BUTTON1 22
#define BUTTON2 24
#define BUTTON3 26
#define BUTTON4 28
#define BUTTON5 30
#define BUTTON6 32
#define BUTTON7 34
#define BUTTON8 36
#define BUTTON9 38
#define BUTTON10 40
#define BUTTON11 42
#define BUTTON12 44
#define BUTTON13 46
#define BUTTON14 50
#define BUTTON15 48
#define BUTTON16 52

#define NUM_LEDS 16
#define MAX_SCENE_LED 9

#define LED1 23
#define LED2 25
#define LED3 27
#define LED4 29
#define LED5 31
#define LED6 33
#define LED7 35
#define LED8 37
#define LED9 39
#define LED10 41
#define LED11 43
#define LED12 45
#define LED13 47
#define LED14 49
#define LED15 51
#define LED16 53

#elif (BOARD == BOARD_UNO) /* for quick testing only 4 buttons mapping */
#define MIDI_PORT Serial
#define NUM_BUTTONS 4
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4
#define BUTTON4 5

#define NUM_LEDS 4
#define MAX_SCENE_LED 3

#define LED1 6
#define LED2 7
#define LED3 8
#define LED4 9
#else
#error "UNKNOWN BOARD."
#endif
