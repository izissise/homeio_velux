#include "TelegramBot.hpp"

TelegramBot::TelegramBot(TimerManager& tm, String const& telegramToken, uint16_t checkIntervalMs)
: _bot(telegramToken, _wifiClient), _messagesHandler([](UniversalTelegramBot&, int) {}) {
  tm.every(checkIntervalMs * 1000, [this]() {
    int numNewMessages = 0;
    while (((numNewMessages = _bot.getUpdates(_bot.last_message_received + 1)) > 0)
      && (numNewMessages <= HANDLE_MESSAGES)) {
      _messagesHandler(_bot, numNewMessages);
    }
  });
}

void TelegramBot::run() {
}

