#include "TimerManager.hpp"

#include <Arduino.h>


void TimerManager::every(uint32_t period, void (*callback)(), int16_t repeatCount){
  const int8_t idx = findFreeEventIndex();
  _events[idx].period = period;
  _events[idx].repeatCount = repeatCount;
  _events[idx].lastTime = millis();
  _events[idx].func = callback;
}

void TimerManager::every(uint32_t period, void (*callback)()) {
  every(period, callback, -1);
}

void TimerManager::after(uint32_t duration, void (*callback)()) {
  every(duration, callback, 1);
}


void TimerManager::update() {
  update(millis());
}

void TimerManager::update(uint32_t now) {
  for (int8_t i = 0; i < maxNumberOfEvents; ++i)  {
    if (_events[i].period > 0) { // Event is setup
      if (_events[i].lastTime + _events[i].period <= now) { // Need trigger
        _events[i].lastTime = now;
        if (_events[i].repeatCount > 0) {
          _events[i].repeatCount -= 1;
          if (_events[i].repeatCount == 0) {
            _events[i].period = 0;
          }
        }
        _events[i].func();
      }
    }
  }
}

int8_t TimerManager::findFreeEventIndex() {
  for (int8_t i = 0; i < maxNumberOfEvents; ++i)  {
    if (_events[i].period == 0)
      return i;
  }
  return -1;
}
