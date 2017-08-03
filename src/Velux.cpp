#include "Velux.hpp"

#include <Arduino.h>

Velux::Velux() {
  _sending = false;
  _signal = 0;
  pinMode(dataPin, OUTPUT);
  switchSignal();
}

void Velux::handleSignal() {
  _tickCount = _tickCount + 1;
  if (!_sending)
    return;
  if (_sepa) {
    if (_tickCount - _lastChangeTick >= 4) {
      _lastChangeTick = _tickCount;
      _sepa = 0;
      switchSignal();
    }
  } else {
    uint8_t bit = ((_data) >> (8 - _pos));
    if (_tickCount - _lastChangeTick >= 2) {
      if (bit == 0 || (_tickCount - _lastChangeTick >= 4))
        _lastChangeTick = _tickCount;
      _sepa = 1;
      switchSignal();
      _pos = _pos + 1;
      if (_pos > 8) {
        _sending = 0;
      }
    }
  }
}

void Velux::switchSignal() {
  _signal = _signal ? 0 : 1;
  digitalWrite(dataPin, _signal);
}

void Velux::passTimeManager(TimerManager& tm) {
  tm.every(picTimeuS / 2, [](void* arg) {
    Velux* v = static_cast<Velux*>(arg);
    v->handleSignal();
  }, static_cast<void*>(this));
}

void Velux::run() {
  if (_sending)
    return;

  uint8_t _data = s4624Proto(Rotor::M2, Way::UP);
  if (_signal == 0)
    switchSignal();

  _pos = 0;
  _sepa = 1;
  _lastChangeTick = 0;
  _tickCount = 0;

  _sending = true;
}
