#include "WebServer.hpp"

WebServer::WebServer() {
  this->onNotFound([this]() { this->send(200, "text/plain", "Not found"); });

  MDNS.addService("http", "tcp", 80);
}

void WebServer::run() {
  this->handleClient();
}
