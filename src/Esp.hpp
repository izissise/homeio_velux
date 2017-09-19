#ifndef _ESP_HPP_
#define _ESP_HPP_

#include <Arduino.h>

#include <memory>
#include <functional>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>

#include "TimerManager.hpp"

#include "IJob.hpp"

class Esp {
public:
  Esp(String const& hostname, String const& ApSsid,
      String const& ApPass, std::function<std::unique_ptr<IJob>(TimerManager&)> createJob);
  virtual ~Esp() = default;

  virtual void run(); // Call in loop()

  bool isConnected() const { return _connected; };

  void displayConnectionInfos() const;

  void _apCallback();
  void _newconfCallback();

protected:
  String _hostname;
  bool _connected;

  TimerManager _timerManager;
  std::unique_ptr<IJob> _job;
  WiFiManager _wifiManager;
};

#endif
