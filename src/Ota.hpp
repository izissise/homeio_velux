#ifndef _OTA_HPP_
#define _OTA_HPP_

#include "WebServer.hpp"

#include <ESP8266HTTPUpdateServer.h>

#include "IJob.hpp"

class Ota : public IJob {
public:
  Ota(WebServer& svr, String username, String password, String updatePath = "/update");
  virtual ~Ota() = default;

  void run() override;

private:
  String _username;
  String _password;
  String _path;

  ESP8266HTTPUpdateServer _httpUpdater;
};

#endif
