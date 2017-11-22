//#include <FS.h> //this needs to be first, or it all crashes and burns...

#include "Esp.hpp"

#include "Timer.hpp"
#include "WebServer.hpp"
#include "WSServer.hpp"
#include "WebSocketSerialConsole.hpp"
#include "Ota.hpp"
#include "TelegramBot.hpp"
#include "Velux.hpp"

#include <memory>

#define BAUD_RATE 115200
#define HOSTNAME "velux"
#define WIFIAPSSID "velux"
#define WIFIAPPASS "esp8266wifi"
#define TELEGRAMBOTTOKEN ""

std::unique_ptr<Esp> esp;
Timer::Timer isAlive(500000, -1);

void setup() {
  delay(1);
  //   Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    yield();
  }
  ESP.eraseConfig();
  Serial.print(F("\r\n---------Starting ESP8266----------\r\nReboot reason -> "));
  Serial.println(ESP.getResetInfo());

  esp.reset(new Esp(HOSTNAME, WIFIAPSSID, WIFIAPPASS));
  Serial.println(F("Connected!"));

  Serial.print(F("Entering Ota grace period\r"));
  auto tm = std::make_shared<Timer::TimerManager>();
  auto wsvr = std::make_shared<WebServer>();
  auto ota = std::make_shared<Ota>(*wsvr, WIFIAPSSID, WIFIAPPASS);
//   auto wss = std::make_shared<WSServer>(81);
//   auto wsserial = std::make_shared<WSSerialConsole>(*wsvr, *wss);
  auto telegramBot = std::make_shared<TelegramBot>(TELEGRAMBOTTOKEN);
  auto velux = std::make_shared<Velux>(*wsvr, *telegramBot, 14);

  esp->addJob(tm);
  esp->addJob(wsvr);
  esp->addJob(ota);

  tm->attachTimer(Timer::Timer{15000000}, [telegramBot, velux]() {
//     esp->addJob(wss);
//     esp->addJob(wsserial);
    esp->addJob(telegramBot);
    esp->addJob(velux);
    Serial.println(F("\rSetup done!               "));
  });
}

void loop() {
  uint32_t now = micros();
  if (isAlive.update(now)) {
    esp->tcpCleanup(); // Cleanup tcp sockets
    Serial.print("."); // Blink using serial
  }
  esp->run();
}
