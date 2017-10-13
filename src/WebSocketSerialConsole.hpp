#ifndef _WEBSOCKETSERIALCONSOLE_HPP_
#define _WEBSOCKETSERIALCONSOLE_HPP_

#include <Arduino.h>

#include "WebServer.hpp"
#include "WSServer.hpp"

#include "IJob.hpp"

constexpr size_t serialBufferSize = 8192;

class WSSerialConsole : public IJob {
public:
  WSSerialConsole(WebServer& svr, WSServer& ws);
  virtual ~WSSerialConsole() = default;

  void run() override;

protected:
  void _returnConsolePage(WebServer& svr);
  void _socketEvent(WSServer& ws, uint8_t num, WStype_t type, uint8_t* payload, size_t length);
  void _serialEvent();

protected:
  WSServer& _ws;
//   int socketClientIds[WEBSOCKETS_SERVER_CLIENT_MAX] _clients;
  int _client; // One client for now
  char _serialBuffer[serialBufferSize];
  unsigned int _bufferSize;

};

#endif
