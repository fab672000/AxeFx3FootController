#pragma once
#include <SC_Button.h>

void layout1() {

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
            turnOnSceneLed (currentSwitch + 1);
          }
          break;

        // Switch 5 (Preset Decrement)
        case 4:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.sendPresetDecrement();
            flashLed( leds[currentSwitch], PEDAL_ACTIVE_FLASH );
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
            turnOnSceneLed (currentSwitch + 1);
          }
          break;

        // Switch 10 (Preset Increment)
        case 9:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.sendPresetIncrement();
            flashLed( leds[currentSwitch], PEDAL_ACTIVE_FLASH );
          }
          break;

        // Switch 11 (Looper Record)
        case 10:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.getLooper().record();
            digitalWrite( LED11, HIGH);
            digitalWrite( LED12, LOW);
            digitalWrite( LED13, LOW);
            //turnOffSceneLeds();
            lcd.setCursor(0, 0);
            lcdPrint("RECORD              ");


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
            digitalWrite( LED11, LOW);
            digitalWrite( LED12, HIGH);
            digitalWrite( LED13, LOW);
            lcd.setCursor(0, 0);
            lcdPrint("PLAY                ");
          }
          break;

        // Switch 13 (Looper Undo)
        case 12:
          if (Axe.isTunerEngaged()) {
            Axe.toggleTuner();
          }
          else {
            Axe.getLooper().undo();
            digitalWrite( LED11, LOW);
            digitalWrite( LED12, LOW);
            digitalWrite( LED13, HIGH);
            turnOffSceneLeds();
            lcd.setCursor(0, 0);
            lcdPrint("UNDO                ");
          }
          break;

        // Switch 14 - Swapped to 15 (TUNER ENGAGE)
        case 13:
          Serial.println("Switch-15 (14) pressed");

          Axe.toggleTuner();

          digitalWrite(LED15, Axe.isTunerEngaged() ? HIGH : LOW);

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

}
