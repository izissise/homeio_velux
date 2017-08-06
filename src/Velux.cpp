#include "Velux.hpp"

#include <Arduino.h>

Velux::Velux()
: _server(80), _rotor(""), _way("") {
  _data = nullptr;
  _wantedData = nullptr;
  _sending = false;
  _signal = 0;
  _timeSent = 0;
  pinMode(dataPin, OUTPUT);
  switchSignal();
  _server.on("/", [this]() { _handleRoot(); });
  _server.on("/velux", [this]() { _request(); });
  _server.onNotFound([this]() { _server.send(200, "text/plain", "Not found"); });
  _server.begin();
}

void Velux::handleSignal() {
  _tickCount = _tickCount + 1;
  if (!_sending) {
    if (_tickCount > 253 && _signal == 0) {
      switchSignal();
    }
    if (_wantedData != _data) {
      if (_signal == 0)
        switchSignal();

      _pos = 0;
      _tickCount = 0;
      _data = _wantedData;
      _sending = true;
    }
    return;
  }
  if (_data[_pos] == 0) {
    if (_timeSent == 1) {
      _sending = false;
      _timeSent = 0;
    } else {
      _pos = 0;
      _timeSent += 1;
    }
    return;
  }
  if (_tickCount >= ((uint16_t)_data[_pos] + (uint16_t)(_timeSent * 650))) {
    switchSignal();
    _pos = _pos + 1;
  }
}

void Velux::switchSignal() {
  _signal = _signal ? 0 : 1;
  digitalWrite(dataPin, _signal);
}

void Velux::passTimeManager(TimerManager& tm) {
  tm.every(tickInus, [](void* arg) {
    Velux* v = static_cast<Velux*>(arg);
    v->handleSignal();
  }, static_cast<void*>(this));
}

void Velux::run() {
  _server.handleClient();
}

void Velux::_handleRoot() {
  _server.send(200, "text/plain", "Rotor: " + _rotor + " Way: " + _way);
}

void Velux::_request() {
  auto rotor = _server.arg("rotor");
  auto way = _server.arg("way");
  Rotor r = (rotor == "M1") ? Rotor::M1 : (rotor == "M2") ? Rotor::M2 : (rotor == "M3") ? Rotor::M3 : Rotor::M3;
  Way w = (way == "UP") ? Way::UP : (way == "DOWN") ? Way::DOWN : (way == "STOP") ? Way::STOP : Way::STOP;
  if ((r == Rotor::M3 && rotor != "M3") || (w == Way::STOP && way != "STOP")) {
    _server.send(409, "text/plain", "Error");
    return;
  }
  _rotor = rotor;
  _way = way;
  _wantedData = s4624Proto(r, w);
  _server.send(200, "text/plain", "Ok");
}


