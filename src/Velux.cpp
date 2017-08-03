#include "Velux.hpp"

#include <Arduino.h>

Velux::Velux()
: _server(80) {
  _data = nullptr;
  _wantedData = nullptr;
  _sending = false;
  _signal = 1;
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
    if (_wantedData != _data) {
      if (_signal == 1)
        switchSignal();

      _pos = 0;
      _tickCount = 0;
      _data = _wantedData;
      _sending = true;
    }
    return;
  }
  if (_data[_pos] == 0) {
    _sending = false;
    return;
  }
  if (_tickCount >= _data[_pos]) {
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
  _server.send(200, "text/plain", String(_signal));
}

void Velux::_request() {
  auto rotor = _server.arg("rotor");
  auto way = _server.arg("way");
  Rotor r = (rotor == "M1") ? Rotor::M1 : (rotor == "M2") ? Rotor::M2 : Rotor::M3;
  Way w = (way == "UP") ? Way::UP : (way == "DOWN") ? Way::DOWN : Way::STOP;
  _wantedData = s4624Proto(r, w);
  _server.send(200, "text/plain", "ok");
}


