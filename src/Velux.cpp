#include "Velux.hpp"

#include <Arduino.h>

Velux::Velux() {
  done = false;
  pinMode(dataPin, OUTPUT);
}

void Velux::switchSignal() {
  digitalWrite(dataPin, _signal);
  _signal = _signal ? 0 : 1;
}

void Velux::run(TimerManager& tm) {
  if (done)
    return;

  uint8_t data = s4624Proto(Rotor::M2, Way::UP);

  _signal = 1;
  switchSignal();
  tm.after(separatorTimeuS, []() {
    switchSignal();
  });

  done = true;
}
