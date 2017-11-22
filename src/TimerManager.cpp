#include "TimerManager.hpp"

#include <Arduino.h>

namespace Timer {

Timer every(uint32_t period, int16_t repeatCount) {
  return Timer{period, repeatCount};
}

Timer every(uint32_t period) {
  return every(period, -1);
}

Timer after(uint32_t duration) {
  return every(duration, 1);
}

Timer::Timer(uint32_t period, int16_t repeatCount)
: _period(period), _lastTime(micros()), _repeatCount(repeatCount) {
}

bool Timer::update(uint32_t now) {
  bool needTrigger = false;
  if (_repeatCount != 0) { // Event is setup
    uint32_t at;
    if (_lastTime >= (nowMaxValue - _period)) { // would overflow
      at = _lastTime - (nowMaxValue / 2);
      at += _period;
      if (at <= (now - (nowMaxValue / 2))) { // Need trigger
        needTrigger = true;
      }
    } else {
      at = _lastTime + _period;
      if (at <= now) { // Need trigger
        needTrigger = true;
      }
    }
    if (needTrigger) { // Setup for next trigger
      _lastTime = now;
      if (_repeatCount > 0) {
        _repeatCount -= 1;
      }
    }
  }
  return needTrigger;
}

bool Timer::update() {
  return update(micros());
}

bool Timer::waitNextUpdate() {
  if (_period == 0)
    return false;
  //TODO use delay() if more than 1ms
  while (!update()) {}
  return true;
}


TimerManager::TimerManager()
: _nAt(0) {
}

void TimerManager::attachTimer(Timer timer, std::function<void()> callback) {
  const int8_t idx = findFreeEventIndex();
  _events[idx] = {timer, callback};
  _nAt = 0;
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
  _nAt = now;
  for (int8_t i = 0; i < maxNumberOfEvents; ++i)  {
    if (_events[i].timer.repeatLeft() != 0) { // Event is setup
      if (_events[i].timer.update(now)) {
        _events[i].cb();
        uint32_t nextTrigger = _events[i].timer.nextTriggerus(now);
        if (nextTrigger < _nAt) {
          _nAt = nextTrigger;
        }
      }
    }
  }
}

int8_t TimerManager::findFreeEventIndex() {
  for (int8_t i = 0; i < maxNumberOfEvents; ++i)  {
    if (_events[i].timer.repeatLeft() == 0)
      return i;
  }
  return -1;
}

};
