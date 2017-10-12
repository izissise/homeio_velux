#include "Velux.hpp"

Velux::Velux(ESP8266WebServer& svr, TimerManager& tm, String const& telegramToken)
: _bot(telegramToken, _wifiClient) {
  _sending = false;
  _data = s4624Proto(Rotor::M1, Way::STOP);
  _needSend = true;
  _megaSignalStartValue = signalStartValue;
  _signal = _megaSignalStartValue;
  _tickCount = 0;
  _timeSent = 0;
  pinMode(dataPin, OUTPUT);
  switchSignal();
  svr.on("/", [this, &svr]() { _handleRoot(svr); });
  svr.on("/velux", [this, &svr]() { _request(svr); });
  svr.on("/megaswitch", [this, &svr]() {
    char buf[2];
    buf[0] = _megaSignalStartValue + '0';
    buf[1] = '\0';
    _megaSignalStartValue = _megaSignalStartValue ? 0 : 1;
    svr.send(200, "text/plain", buf);
  });
  svr.on("/switch", [this, &svr]() {
    char buf[2];
    buf[0] = _signal + '0';
    buf[1] = '\0';
    switchSignal();
    svr.send(200, "text/plain", buf);
  });

  tm.every(tickInus, [this]() {
    handleSignal();
  });

  _userChatIds.push_back("87098341");
}

void Velux::handleSignal() {
  _tickCount = _tickCount + 1;
  if (!_sending) {
    if (_tickCount == timeoutLastCheckTick && (_megaSignalStartValue ? (_signal == 0) : (_signal == 1))) {
      switchSignal();
    }
    if (_needSend) {
      if (_signal == _megaSignalStartValue)
//         noInterrupts(); // Disable interrupt

      _pos = 0;
      _tickCount = 0;
      _needSend = false;
      _sending = true;
    }
    return;
  }
  if (_data[_pos] == 0) {
    if (_timeSent == 1) {
      _sending = false;
      _timeSent = 0;
//       interrupts(); // Enable interrupt
    } else {
      _pos = 0;
      _timeSent += 1;
    }
    return;
  }
  if (_tickCount >= (_data[_pos] + (_timeSent * backupSignalStartTick))) {
    switchSignal();
    _pos = _pos + 1;
  }
}

void Velux::switchSignal() {
  _signal = _signal ? 0 : 1;
  digitalWrite(dataPin, _signal);
}

void Velux::run() {
  if (_sending == false) {
    if (millis() > static_cast<size_t>(_Bot_lasttime + botMtbs))  {
      int numNewMessages = 0;
      while ((numNewMessages = _bot.getUpdates(_bot.last_message_received + 1)) > 0) {
        handleNewMessages(numNewMessages);
      }
      _Bot_lasttime = millis();
    }
  }
}

void Velux::_handleRoot(ESP8266WebServer& svr) {
  svr.send(200, "text/plain", "I'm up");
}

void Velux::_request(ESP8266WebServer& svr) {
  auto rotor = svr.arg("rotor");
  auto way = svr.arg("way");
  Rotor r = (rotor == "M1") ? Rotor::M1 : (rotor == "M2") ? Rotor::M2 : (rotor == "M3") ? Rotor::M3 : Rotor::M3;
  Way w = (way == "UP") ? Way::UP : (way == "DOWN") ? Way::DOWN : (way == "STOP") ? Way::STOP : Way::STOP;
  if ((r == Rotor::M3 && rotor != "M3") || (w == Way::STOP && way != "STOP")) {
    svr.send(409, "text/plain", "Error");
    return;
  }
  _data = s4624Proto(r, w);
  _needSend = true;
  svr.send(200, "text/plain", "Ok");
}


void Velux::handleNewMessages(int numNewMessages) {
  for(int i=0; i<numNewMessages; i++) {
    String chat_id = String(_bot.messages[i].chat_id);
    String text = _bot.messages[i].text;
    if (text == F("/start")) {
      String welcome = "Hello, you can now control the velux.\n";
      welcome += "/stop : To stop anything happening\n";
      welcome += "/open : To open the velux\n";
      welcome += "/close : To close the velux\n";
      welcome += "/rain : To close the rain protector\n";
      welcome += "/sunny : To open the rain protector\n";
      welcome += "/morelight : To open the sun protector\n";
      welcome += "/lesslight : To close the sun protector\n";
      welcome += "/status : To get a status\n";
      _bot.sendMessage(chat_id, welcome, "Markdown");
      if (std::find(std::begin(_userChatIds), std::end(_userChatIds), chat_id) == std::end(_userChatIds))
        _userChatIds.push_back(chat_id);
    } else if (text == "/stop") {
      _data = s4624Proto(Rotor::M1, Way::STOP);
      _needSend = true;
    } else if (text == "/open") {
      _data = s4624Proto(Rotor::M2, Way::DOWN);
      _needSend = true;
    } else if (text == "/close") {
      _data = s4624Proto(Rotor::M2, Way::UP);
      _needSend = true;
    } else if (text == "/rain") {
      _data = s4624Proto(Rotor::M3, Way::UP);
      _needSend = true;
    } else if (text == "/sunny") {
      _data = s4624Proto(Rotor::M3, Way::DOWN);
      _needSend = true;
    } else if (text == "/morelight") {
      _data = s4624Proto(Rotor::M1, Way::UP);
      _needSend = true;
    } else if (text == "/lesslight") {
      _data = s4624Proto(Rotor::M1, Way::DOWN);
      _needSend = true;
    } else if (text == "/status") {
      _bot.sendMessage(chat_id, "Just look at it to know", "");
    } else {
      _bot.sendMessage(chat_id, text, "");
    }
  }
}
