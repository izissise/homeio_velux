#include "Velux.hpp"

#include <Arduino.h>

Velux::Velux() {
  _sending = false;
  _signal = 1;
  pinMode(dataPin, OUTPUT);
  switchSignal();
}

void Velux::handleSignal() {
  _tickCount = _tickCount + 1;
  if (!_sending) {
    if (_wantedData != _data) {
      if (_signal == 1)
        switchSignal();

      _pos = 0;
      _sepa = 1;
      _lastChangeTick = 0;
      _tickCount = 0;
      _data = _wantedData;
      _sending = true;
    }
    return;
  }
  if (_sepa) {
    if (_tickCount - _lastChangeTick >= 6) {
      _lastChangeTick = _tickCount;
      _sepa = 0;
      switchSignal();
    }
  } else {
    uint8_t bit = ((_data) >> (7 - _pos)) & 0x01;
    if (_tickCount - _lastChangeTick >= 2) {
      if (bit == 0 || ((_tickCount - _lastChangeTick) >= 4)) {
        _lastChangeTick = _tickCount;
        _pos = _pos + 1;
        _sepa = 1;
      }
      switchSignal();
      if (_pos > 7) {
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
  uint8_t _wantedData = s4624Proto(Rotor::M2, Way::UP);
}
