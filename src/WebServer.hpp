#ifndef _WEBSERVER_HPP_
#define _WEBSERVER_HPP_

#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "IJob.hpp"

class WebServer : public ESP8266WebServer, public IJob {
public:
  WebServer();
  virtual ~WebServer() = default;

  void run() override;
};

#endif

