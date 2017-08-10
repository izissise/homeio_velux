#ifndef _VELUX_HPP_
#define _VELUX_HPP_

#include <ESP8266WebServer.h>

#include "IJob.hpp"

#include "S4624Proto.hpp"

constexpr int8_t dataPin = 2;

class Velux : public IJob {
public:
  Velux();
  ~Velux() = default;

  void passTimeManager(TimerManager& tm) override;
  void run() override;

  void handleSignal();

private:
  void switchSignal();

  void _handleRoot();
  void _request();

private:
  ESP8266WebServer _server;

private:
  uint8_t _signal; // Electric signal value
  uint16_t _timeSent;
  bool _sending; // Is sending

  uint8_t _pos;
  uint16_t _tickCount;
  uint16_t* _data;
  uint16_t* _wantedData;

  String _rotor;
  String _way;
};

#endif
