#include "TimerManager.hpp"

#include <Arduino.h>

TimerManager::TimerManager() {
  _nAt = 0;
  for (uint8_t i = 0; i < maxNumberOfEvents; ++i) {
    _events[i].repeatCount = 0;
  }
}

void TimerManager::every(uint32_t period, std::function<void()> callback, int16_t repeatCount){
  const int8_t idx = findFreeEventIndex();
  _events[idx].period = period;
  _events[idx].repeatCount = repeatCount;
  _events[idx].lastTime = micros();
  _events[idx].func = callback;
  _nAt = 0;
}

void TimerManager::every(uint32_t period, std::function<void()> callback) {
  every(period, callback, -1);
}

void TimerManager::after(uint32_t duration, std::function<void()> callback) {
  every(duration, callback, 1);
}


void TimerManager::update() {
  update(micros());
}

void TimerManager::run() {
  update();
}

void TimerManager::update(uint32_t now) {
  if (now < _nAt)
    return;
  uint32_t at;
  for (int8_t i = 0; i < maxNumberOfEvents; ++i)  {
    if (_events[i].repeatCount != 0) { // Event is setup
      bool needTrigger = false;
      if (_events[i].lastTime >= (nowMaxValue - _events[i].period)) { // would overflow
        _nAt = 0;
        at = _events[i].lastTime - (nowMaxValue / 2);
        at += _events[i].period;
        if (at <= (now - (nowMaxValue / 2))) { // Need trigger
          needTrigger = true;
        }
      } else {
        at = _events[i].lastTime + _events[i].period;
        if (at <= now) { // Need trigger
          needTrigger = true;
        }
      }
      if (needTrigger) {
        _events[i].lastTime = now;
        if (_events[i].repeatCount > 0) {
          _events[i].repeatCount -= 1;
        }
        _nAt = now;
        _events[i].func();
      } else {
        if (at < _nAt) {
          _nAt = at;
        }
      }
    }
  }
}

int8_t TimerManager::findFreeEventIndex() {
  for (int8_t i = 0; i < maxNumberOfEvents; ++i)  {
    if (_events[i].repeatCount == 0)
      return i;
  }
  return -1;
}
