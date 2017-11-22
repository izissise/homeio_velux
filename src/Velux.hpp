#ifndef _VELUX_HPP_
#define _VELUX_HPP_

#include <vector>
#include <algorithm>

#include <Arduino.h>

#include "IJob.hpp"
#include "TimerManager.hpp"
#include "TelegramBot.hpp"
#include "WebServer.hpp"

#include "S4624Proto.hpp"

constexpr uint16_t backupSignalStartTick = 2010;
constexpr uint16_t timeoutLastCheckTick = 7168;

constexpr uint8_t signalStartValue = 0;

class Velux : public IJob {
public:
  Velux(WebServer& svr, TelegramBot& tBot, int gpioPin = 2);
  virtual ~Velux() = default;

  void run() override;

  void handleSignal();

private:
  inline void switchSignal();

  void _handleRoot(WebServer& svr);
  void _request(WebServer& svr);
  void _handleNewMessages(UniversalTelegramBot& bot, int numNewMessages);

private:
  Timer::Timer _ticker;
  int8_t _gpioPin;
  uint8_t _signal; // Electric signal value
  uint8_t _megaSignalStartValue;
  uint16_t _timeSent;
  bool _sending; // Is sending

  uint8_t _pos;
  uint16_t _tickCount;
  bool _needSend;
  const uint16_t* _data;
};

#endif
