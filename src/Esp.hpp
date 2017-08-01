#ifndef _ESP_HPP_
#define _ESP_HPP_

#include <Arduino.h>

#include <string>
#include <memory>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>

#include "Ota.hpp"

#include "IJob.hpp"

#include "DeepSleep.hpp"

class Esp {
public:
  Esp(std::string const& hostname, std::string const& ApSsid,
      std::string const& ApPass, std::unique_ptr<IJob> job);
  virtual ~Esp() = default;

  virtual void run(); // Call in loop()

  bool isConnected() const { return _connected; };

  void _apCallback();
  void _connectCallback();

protected:
  std::string _hostname;
  bool _connected;

  std::unique_ptr<IJob> _job;
  WiFiManager _wifiManager;
};

#endif
