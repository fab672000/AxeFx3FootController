#include "TimerUtils.h"

static Timer timer; // unique instance for all timer services

Timer& TimerUtils::GetTimer() {
    return timer;
}
