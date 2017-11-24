#include "TelegramBot.hpp"

TelegramBot::TelegramBot(String const& telegramToken, uint16_t checkIntervalMs)
: _bot(telegramToken, _wifiClient), _timer(checkIntervalMs * 1000, -1), _messagesHandler([](UniversalTelegramBot&, int) {}){
}

void TelegramBot::run() {
  if (_timer.update()) {
    int numNewMessages = 0;
    while (((numNewMessages = _bot.getUpdates(_bot.last_message_received + 1)) > 0)
      && (numNewMessages <= HANDLE_MESSAGES)) {
      _messagesHandler(_bot, numNewMessages);
      }
  }
}
