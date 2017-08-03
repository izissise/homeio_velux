#ifndef _VELUX_HPP_
#define _VELUX_HPP_

#include "IJob.hpp"

#include "S4624Proto.hpp"

constexpr int8_t dataPin = 2;
constexpr uint32_t separatorTimeuS = 1260;
constexpr uint32_t picTimeuS = 416;

class Velux : public IJob {
public:
  Velux();
  ~Velux() = default;

  void passTimeManager(TimerManager& tm) override;
  void run() override;

  void handleSignal();

private:
  void switchSignal();

private:
  uint8_t _signal;
  bool _sending;
  uint8_t _data;

  uint8_t _pos;
  uint8_t _sepa;
  uint8_t _lastChangeTick;
  uint8_t _tickCount;
};

#endif
