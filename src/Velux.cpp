#include "Velux.hpp"

#include <Arduino.h>

Velux::Velux(TimerManager& tm)
: _server(80), _rotor(""), _way("") {
  _data = nullptr;
  _sending = false;
  _needSend = false;
  _megaSignalStartValue = signalStartValue;
  _signal = _megaSignalStartValue;
  _tickCount = 0;
  _timeSent = 0;
  pinMode(dataPin, OUTPUT);
  switchSignal();
  _server.on("/", [this]() { _handleRoot(); });
  _server.on("/velux", [this]() { _request(); });
  _server.on("/megaswitch", [this]() {
    char buf[2];
    buf[0] = _megaSignalStartValue + '0';
    buf[1] = '\0';
    _megaSignalStartValue = _megaSignalStartValue ? 0 : 1;
    _server.send(200, "text/plain", buf);

  });
  _server.on("/switch", [this]() {
    char buf[2];
    buf[0] = _signal + '0';
    buf[1] = '\0';
    switchSignal();
    _server.send(200, "text/plain", buf);

  });
  _server.onNotFound([this]() { _server.send(200, "text/plain", "Not found"); });
  _server.begin();

  tm.every(tickInus, [this]() {
    handleSignal();
  });
}

void Velux::handleSignal() {
  _tickCount = _tickCount + 1;
  if (!_sending) {
    if (_tickCount == timeoutLastCheckTick && (_megaSignalStartValue ? (_signal == 0) : (_signal == 1))) {
      switchSignal();
    }
    if (_needSend) {
      if (_signal == _megaSignalStartValue)
        noInterrupts(); // Disable interrupt
        switchSignal();

      _pos = 0;
      _tickCount = 0;
      _needSend = false;
      _sending = true;
    }
    return;
  }
  if (_data[_pos] == 0) {
    if (_timeSent == 1) {
      _sending = false;
      _timeSent = 0;
      interrupts(); // Enable interrupt
    } else {
      _pos = 0;
      _timeSent += 1;
    }
    return;
  }
  if (_tickCount >= (_data[_pos] + (_timeSent * backupSignalStartTick))) {
    switchSignal();
    _pos = _pos + 1;
  }
}

void Velux::switchSignal() {
  _signal = _signal ? 0 : 1;
  digitalWrite(dataPin, _signal);
}

void Velux::run() {
  if (_sending == 0) {
    _server.handleClient();
  }
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
  _data = s4624Proto(r, w);
  _needSend = true;
  _server.send(200, "text/plain", "Ok");
}


