#include "Esp.hpp"

#include <functional>

static Esp* gEsp = nullptr;

Esp::Esp(std::string const& hostname, std::string const& ApSsid,
         std::string const& ApPass, std::unique_ptr<IJob> job)
: _hostname(hostname), _connected(false), _job(std::move(job)) {
  if (gEsp) {
    Serial.println("An Esp object have already been created -> reset");
    ESP.reset();
  }
  gEsp = this;
  _wifiManager.setDebugOutput(false);
  _wifiManager.setConfigPortalTimeout(3600); //sets timeout until configuration portal gets turned off in seconds
  _wifiManager.setAPCallback([] (WiFiManager*) { gEsp->_apCallback(); });
  _wifiManager.setSaveConfigCallback([] () { gEsp->_connectCallback(); });
  if (!_wifiManager.autoConnect(ApSsid.c_str(), ApPass.c_str())) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  setupOta();
  _job->passTimeManager(_timerManager);
}

void Esp::run() {
  _timerManager.update();
  _job->run();
  handleOta();
}

void Esp::_apCallback() {
  Serial.print("Entered config mode with SSID: ");
  Serial.println(_wifiManager.getConfigPortalSSID());
}

void Esp::_connectCallback() {
  Serial.println("------- Connected ----------");

  _connected = true;
  MDNS.begin(_hostname.c_str());
  Serial.println("Ip: " + WiFi.localIP());
  Serial.println("SSID: " + WiFi.SSID());
  Serial.println("MAC: " + WiFi.macAddress());
}
