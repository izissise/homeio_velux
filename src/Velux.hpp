#ifndef _VELUX_HPP_
#define _VELUX_HPP_

#include <vector>
#include <algorithm>

#include <ESP8266WebServer.h>

#include <UniversalTelegramBot.h>

#include "IJob.hpp"
#include "TimerManager.hpp"

#include "S4624Proto.hpp"

constexpr int8_t dataPin = 2;

constexpr uint16_t backupSignalStartTick = 2010;
constexpr uint16_t timeoutLastCheckTick = 7168;

constexpr uint16_t botMtbs = 500; //mean time between scan messages

constexpr uint8_t signalStartValue = 0;

class Velux : public IJob {
public:
  Velux(TimerManager& tm, String const& telegramToken);
  ~Velux() = default;

  void run() override;

  void handleSignal();

private:
  inline void switchSignal();

  void _handleRoot();
  void _request();

  void handleNewMessages(int numNewMessages);

private:
  ESP8266WebServer _server;
  WiFiClientSecure _wifiClient;
  UniversalTelegramBot _bot;

private:
  uint8_t _signal; // Electric signal value
  uint8_t _megaSignalStartValue;
  uint16_t _timeSent;
  bool _sending; // Is sending

  uint8_t _pos;
  uint16_t _tickCount;
  bool _needSend;
  const uint16_t* _data;

  std::vector<String> _userChatIds;
  uint16_t _Bot_lasttime;   //last time messages' scan has been dones
};

#endif
