//#include <FS.h> //this needs to be first, or it all crashes and burns...

#include "Esp.hpp"

#include "Timer.hpp"
#include "WebServer.hpp"
// #include "WSServer.hpp"
// #include "WebSocketSerialConsole.hpp"
#include "Ota.hpp"
#include "TelegramBot.hpp"
#include "Velux.hpp"

#include <memory>

#define BAUD_RATE 115200
#define LED 15
#define HOSTNAME "velux"
#define WIFIAPSSID "velux"
#define WIFIAPPASS "esp8266wifi"
#define TELEGRAMBOTTOKEN "401430405:AAFNTU-Rm9tAFnm6JUKdQi1c08yQ3MgnCUw"

static std::unique_ptr<Esp> esp;
static Timer::Timer isAlive(500000, -1);
static uint16_t needCleanup = 0;

void setup() {
  delay(100);
  //   Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.begin(BAUD_RATE);
//   while (!Serial) {
//     yield();
//   }
//   ESP.eraseConfig();
  Serial.print(F("\r\n-------Starting ESP8266--------\r\nReboot reason -> "));
  Serial.println(ESP.getResetInfo());

  pinMode(LED, OUTPUT);
  delay(2);
  digitalWrite(LED, HIGH);
  esp.reset(new Esp(HOSTNAME, WIFIAPSSID, WIFIAPPASS));
  Serial.println(F("Connected!"));

  Serial.print(F("Entering Ota grace period\r"));
  auto tm = std::make_shared<Timer::TimerManager>();
  auto wsvr = std::make_shared<WebServer>();
  auto ota = std::make_shared<Ota>(*wsvr, WIFIAPSSID, WIFIAPPASS);
//   auto wss = std::make_shared<WSServer>(81);
//   auto wsserial = std::make_shared<WSSerialConsole>(*wsvr, *wss);
  auto telegramBot = std::make_shared<TelegramBot>(TELEGRAMBOTTOKEN);
  auto velux = std::make_shared<Velux>(*wsvr, *telegramBot, 5);

  esp->addJob(tm);
  esp->addJob(wsvr);
  esp->addJob(ota);

  tm->attachTimer(Timer::after(15000000), [telegramBot, velux]() {
//     esp->addJob(wss);
//     esp->addJob(wsserial);
    esp->addJob(telegramBot);
    esp->addJob(velux);
    Serial.println(F("\rSetup done!                    "));
  });
}

void loop() {
  uint32_t now = micros();
  if (isAlive.update(now)) {
    if (needCleanup == 10) {
      esp->tcpCleanup(); // Cleanup tcp sockets
      needCleanup = 0;
    } else { ++needCleanup; }
    digitalWrite(LED, !digitalRead(LED));
    Serial.print("."); // Blink using serial
  }
  esp->run();
}
