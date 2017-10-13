#include "WebServer.hpp"

WebServer::WebServer(uint16_t port)
: ESP8266WebServer(port) {
  this->onNotFound([this]() { this->send(404, "text/plain", "Not found"); });

  this->begin();
  MDNS.addService("http", "tcp", port);
}

void WebServer::run() {
  this->handleClient();
}
