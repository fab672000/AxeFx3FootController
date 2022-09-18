#pragma once
#include <Arduino.h>

// DEBUG Mode:
#define DEBUG

// BOARDS
#define BOARD_SW16_EXP4        1 /* A board with a large number IO's as the AVR ATMEGA */
#define BOARD_MINI_TESTING     2 /* AVR or other small boards that have at minimum 3K of RAM */
#define BOARD_T40             11 /* TEENSY 4.0 */

#define BOARD   BOARD_SW16_EXP4

// The board below in conjunction maybe with a NO_DISPLAY could be used for an even more minimal floorboard with no display feedback that could fit on an Arduino Nano or UNO board
// #define BOARD  BOARD_MINI_TESTING


// DISPLAYS
// TODO: add more displayys to encapsulate in FcDisplay class
#define NO_DISPLAY            0
#define LIQUID_DISPLAY_I2C    1
#define ADAFRUIT_LCD_SHIELD   2

#define DISPLAY_TYPE LIQUID_DISPLAY_I2C
//#define DISPLAY_TYPE ADAFRUIT_LCD_SHIELD
//#define DISPLAY_TYPE NO_DISPLAY

// MIDI
const byte MidiChannel = 1;

#define P_ Serial.print
#define PL_ Serial.println

#define AXE_REFRESH_RATE 1500

#define PEDAL_ACTIVE_FLASH 50 // Delay for flash when pedal is pressed
#define TAP_TEMPO_LED_DURATION 100
#define DEFAULT_DEBOUNCE 40

#if (BOARD == BOARD_SW16_EXP4) 
#define MIDI_PORT Serial1

// comment out the line below to get direct buttons mapping (no CD4067 mux)
#define HAS_MUX

// Comment out the line below if not use led 74[a]hct595 led shifters
#define HAS_LED_SHIFTERS
#ifdef HAS_LED_SHIFTERS

#define LED_DATA 7
#define LED_CLOCK 8
#define LED_LATCH 9
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define LED5 4
#define LED6 5
#define LED7 6
#define LED8 7
#define LED9 8
#define LED10 9
#define LED11 10
#define LED12 11
#define LED13 12
#define LED14 13
#define LED15 14
#define LED16 15
#else
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
#endif

#define NUM_BUTTONS 16

#ifdef HAS_MUX
#define BUTTON1_16 6
#else
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
#endif
#define NUM_LEDS 16
#define MAX_SCENE_LED 9

#elif (BOARD == BOARD_MINI_TESTING) /* for quick testing only 4 buttons mapping */
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
