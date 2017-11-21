//#include <FS.h> //this needs to be first, or it all crashes and burns...

#include "Esp.hpp"

#include "TimerManager.hpp"
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

  Serial.print(F("  Entering Ota grace period\r"));
  auto tm = std::make_shared<TimerManager>();
  auto wsvr = std::make_shared<WebServer>();
  auto ota = std::make_shared<Ota>(*wsvr, WIFIAPSSID, WIFIAPPASS);
  //   auto wss = std::make_shared<WSServer>(81);
  //   auto wsserial = std::make_shared<WSSerialConsole>(*wsvr, *wss);
  auto telegramBot = std::make_shared<TelegramBot>(*tm, TELEGRAMBOTTOKEN);
  auto velux = std::make_shared<Velux>(*wsvr, *tm, *telegramBot, 14);

  esp->addJob(std::static_pointer_cast<IJob>(tm));
  esp->addJob(std::static_pointer_cast<IJob>(wsvr));
  esp->addJob(std::static_pointer_cast<IJob>(ota));

  esp->tcpCleanup(); // Cleanup tcp sockets
  tm->after(15000000, [telegramBot, velux]() {
  //   esp->addJob(std::static_pointer_cast<IJob>(wss));
  //   esp->addJob(std::static_pointer_cast<IJob>(wsserial));
    esp->addJob(std::static_pointer_cast<IJob>(telegramBot));
    esp->addJob(std::static_pointer_cast<IJob>(velux));
    Serial.println(F("\rSetup done"));
  });

  tm->every(500000, []() { // Show that it is alive
    esp->tcpCleanup(); // Cleanup tcp sockets
    Serial.print("."); // Blink using serial
  });
}

void loop() {
  esp->run();
}
