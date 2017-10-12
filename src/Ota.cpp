#include "Ota.hpp"

static Ota* gOta = nullptr;

Ota::Ota(String hostname, uint16_t port, String password, std::function<void()> onStart, std::function<void(ota_error_t)> onError, std::function<void(unsigned int, unsigned int)> onProgress, std::function<void()> onEnd)
: _onStart(onStart), _onError(onError), _onProgress(onProgress), _onEnd(onEnd) {
  if (gOta) {
    Serial.println("An OTA object have already been created -> reset");
    ESP.reset();
  }
  gOta = this;

  ArduinoOTA.setPort(port);
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPassword(password);

  ArduinoOTA.onStart([]() { gOta->_startCb(); });
  ArduinoOTA.onError([](ota_error_t err) { gOta->_errorCb(err); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) { gOta->_progressCb(progress, total); });
  ArduinoOTA.onEnd([]() { gOta->_endCb(); });

  ArduinoOTA.begin();
  MDNS.addService("http", "tcp", 81);
}

void Ota::run() {
  ArduinoOTA.handle();
}

void Ota::_startCb() {
  if (_onStart) {
    _onStart();
  }
}

void Ota::_errorCb(ota_error_t err) {
  if (_onError) {
    _onError(err);
  }

}

void Ota::_progressCb(unsigned int progress, unsigned int total) {
  if (_onProgress) {
    _onProgress(progress, total);
  }
}

void Ota::_endCb() {
  if (_onEnd) {
    _onEnd();
  }
}
