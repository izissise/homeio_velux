#ifndef _WEBSERVER_HPP_
#define _WEBSERVER_HPP_

#include <vector> // Include vector so Arduino.h doesn't define min and max macros
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "IJob.hpp"

class WebServer : public ESP8266WebServer, public IJob {
public:
  explicit WebServer(uint16_t port = 80);
  virtual ~WebServer() = default;

  void run() override;
};

#endif

