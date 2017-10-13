#include "WSServer.hpp"

WSServer::WSServer(uint16_t port, std::function<void(WSServer& svr, uint8_t num, WStype_t type, uint8_t* payload, size_t length)> handler)
: WebSocketsServer(port) {
  setHandler(handler);
  this->begin();
  MDNS.addService("ws", "tcp", port);
}

void WSServer::setHandler(std::function<void (WSServer&, uint8_t, WStype_t, uint8_t*, size_t)> handler) {
  this->onEvent([this, handler](uint8_t num, WStype_t type, uint8_t* payload, size_t length) { handler(*this, num, type, payload, length); });
}

void WSServer::run() {
  this->loop();
}
//   switch (type) {
//     case WStype_DISCONNECTED:
//       break;
//     case WStype_CONNECTED:
// //    IPAddress ip = this->remoteIP(num);
//       break;
//
//     case WStype_TEXT:
//       break;
//     case WStype_BIN:
//       break;
//     case WStype_ERROR:
//       break;
//     case WStype_FRAGMENT:
//       break;
//     case WStype_FRAGMENT_BIN_START:
//       break;
//     case WStype_FRAGMENT_FIN:
//       break;
//     case WStype_FRAGMENT_TEXT_START:
//       break;
//     default:
//       this->sendTXT(num, "Data type not supported");
//       break;
//   }
