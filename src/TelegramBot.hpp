#ifndef _TELGRAMB_HPP_
#define _TELGRAMB_HPP_

#include <functional>

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "IJob.hpp"
#include "TimerManager.hpp"

constexpr uint16_t botMtbs = 500; //mean time between scan messages

class TelegramBot : public IJob {
public:
  TelegramBot(TimerManager& tm, String const& telegramToken, uint16_t checkIntervalMs = botMtbs);
  virtual ~TelegramBot() = default;

  void run() override;

  void setMessageHandler(std::function<void(UniversalTelegramBot& bot, int idxNewMess)> messageHandler) { _messagesHandler = messageHandler; };
private:
  WiFiClientSecure _wifiClient;
  UniversalTelegramBot _bot;

private:
  std::function<void(UniversalTelegramBot& bot, int idxNewMess)> _messagesHandler;
};

#endif
