#ifndef _VELUX_HPP_
#define _VELUX_HPP_

#include "IJob.hpp"

#include "S4624Proto.hpp"

constexpr int8_t dataPin = 2;
constexpr uint32_t separatorTimeuS = 1260;
constexpr uint32_t picTimeuS = 400;

class Velux : public IJob {
public:
  Velux();
  ~Velux() = default;

  void run(TimerManager& tm) override;
  void switchSignal();

private:
  uint8_t _signal;
  bool done;
};

#endif

