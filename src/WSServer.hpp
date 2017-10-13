#ifndef _WSSERVER_HPP_
#define _WSSERVER_HPP_

#include <cstdint>
#include <functional>

#include <vector> // Include vector so Arduino.h doesn't define min and max macros
#include <Arduino.h>
#include <Hash.h>
#include <ESP8266mDNS.h>

#include <WebSocketsServer.h>

#include "IJob.hpp"

class WSServer : public WebSocketsServer, public IJob {
public:
  WSServer(uint16_t port, std::function<void(WSServer& svr, uint8_t num, WStype_t type, uint8_t* payload, size_t length)> handler = [](WSServer&, uint8_t, WStype_t, uint8_t*, size_t) { });
  virtual ~WSServer() = default;

  void setHandler(std::function<void(WSServer& svr, uint8_t num, WStype_t type, uint8_t* payload, size_t length)> handler);

  void run() override;
};

#endif
