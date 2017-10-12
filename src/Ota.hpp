#ifndef _OTA_HPP_
#define _OTA_HPP_

#include <functional>

#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include "IJob.hpp"

class Ota : public IJob {
public:
  Ota(std::function<void()> onStart = nullptr, std::function<void(ota_error_t)> onError = nullptr, std::function<void(unsigned int, unsigned int)> onProgress = nullptr, std::function<void()> onEnd = nullptr);
  virtual ~Ota() = default;

  void run() override;

public:
  void _startCb();
  void _errorCb(ota_error_t err);
  void _progressCb(unsigned int progress, unsigned int total);
  void _endCb();

private:
  std::function<void()> _onStart;
  std::function<void(ota_error_t)> _onError;
  std::function<void(unsigned int, unsigned int)> _onProgress;
  std::function<void()> _onEnd;
};

#endif
