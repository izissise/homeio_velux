
#include "Ota.hpp"

#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>

ESP8266WebServer updateServer(82);
ESP8266HTTPUpdateServer httpUpdater;

static void onStart() {
  SPIFFS.end();
  Serial.println("OTA Update Starting\n");
}

static void onEnd() {
  SPIFFS.begin();
  Serial.println("OTA Update Complete\n");
}

static void onProgress(unsigned int progress, unsigned int total) {
  Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
}

static void onError(ota_error_t error) {
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) Serial.println("OTA Auth Failed");
  else if (error == OTA_BEGIN_ERROR) Serial.println("OTA Begin Failed");
  else if (error == OTA_CONNECT_ERROR) Serial.println("OTA Connect Failed");
  else if (error == OTA_RECEIVE_ERROR) Serial.println("OTA Receive Failed");
  else if (error == OTA_END_ERROR) Serial.println("OTA End Failed");
}

void setupOta() {
  ArduinoOTA.onStart(&onStart);
  ArduinoOTA.onEnd(&onEnd);
  ArduinoOTA.onProgress(&onProgress);
  ArduinoOTA.onError(&onError);

  ArduinoOTA.begin();
  httpUpdater.setup(&updateServer);
  updateServer.begin();
}

void handleOta() {
  ArduinoOTA.handle();
}
