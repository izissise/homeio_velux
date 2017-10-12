//#include <FS.h> //this needs to be first, or it all crashes and burns...

#include "Esp.hpp"

#include "TimerManager.hpp"
#include "Ota.hpp"
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
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  Serial.println(F("Starting ESP8266"));
  Serial.println(ESP.getResetInfo());

  esp.reset(new Esp(HOSTNAME, WIFIAPSSID, WIFIAPPASS));
  Serial.println(F("Connected!"));

  auto tm = std::make_shared<TimerManager>();
  auto ota = std::make_shared<Ota>(HOSTNAME, 8266, WIFIAPPASS);
  auto velux = std::make_shared<Velux>(*tm, TELEGRAMBOTTOKEN);

  esp->addJob(std::static_pointer_cast<IJob>(tm));
  esp->addJob(std::static_pointer_cast<IJob>(ota));
  esp->addJob(std::static_pointer_cast<IJob>(velux));

  tm->every(500000, []() { // Show that's alive
    Serial.print("."); // Blink using serial
  });
}

void loop() {
  esp->run();
}
