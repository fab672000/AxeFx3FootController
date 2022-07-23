#pragma once
#include <SC_Button.h>


void layoutSelect(){
    // long press function to Switch 11
  if (Buttons[12].pressedFor(2000)) {
    PL_("Switch long pressed");
  }
}
