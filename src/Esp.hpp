#ifndef _ESP_HPP_
#define _ESP_HPP_


#include <Arduino.h>
#include <lwip/tcp_impl.h> // do this before including other stuff like wificlient.h

#include <memory>
#include <functional>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>

#include "Timer.hpp"

#include "IJob.hpp"

constexpr size_t maxNumberOfJob = 10;

class Esp {
public:
  Esp(String const& hostname, String const& ApSsid, String const& ApPass);
  virtual ~Esp() = default;

  virtual void run(); // Call in loop()

  void addJob(std::shared_ptr<IJob> job);

  bool isConnected() const { return _connected; };

  void displayConnectionInfos() const;

  void _apCallback();
  void _newconfCallback();

  void tcpCleanup();

protected:
  String   _hostname;
  bool     _connected;
  unsigned int _jobNumber;

  WiFiManager _wifiManager;
  std::shared_ptr<IJob> _jobs[maxNumberOfJob];
};

#endif
