#include "DeepSleep.hpp"

#include <ESP8266WiFi.h>

void enterDeepSleep(int seconds) {
  ESP.deepSleep(seconds * 1000000); // Deep Sleep
}
