//#include <FS.h> //this needs to be first, or it all crashes and burns...

#include "Esp.hpp"

#include "Velux.hpp"

#include <memory>

#define BAUD_RATE 115200
#define HOSTNAME "velux"
#define WIFIAPSSID "velux"
#define WIFIAPPASS "esp8266wifi"

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

  esp.reset(new Esp(HOSTNAME, WIFIAPSSID, WIFIAPPASS, [](TimerManager& tm) {
    return std::unique_ptr<IJob>(new Velux(tm, TELEGRAMBOTTOKEN));
  }));
  if (!esp) {
    Serial.println(F("Error starting bye"));
    ESP.reset();
  }
  Serial.println(F("Setup done!"));
}

void loop() {
  esp->run();
}
