#ifndef _TIMERMANAGER_HPP_
#define _TIMERMANAGER_HPP_

#include <cstdint>
#include <functional>

#include "IJob.hpp"

namespace Timer {

constexpr uint32_t nowMaxValue = 0xffffffff;
constexpr int8_t maxNumberOfEvents = 4;

class Timer {
public:
  void update();
  void update(uint32_t now);

  bool triggerAnd();
protected:
  uint32_t period = 0;
  uint32_t lastTime = 0;
  int16_t repeatCount = 0;
  std::function<void()> func;
};

Timer every(uint32_t period, std::function<void()> callback);
Timer every(uint32_t period, std::function<void()> callback, int16_t repeatCount);
Timer after(uint32_t duration, std::function<void()> callback);

class TimerManager : public IJob {
public:
  TimerManager();
  virtual ~TimerManager() = default;

  void every(uint32_t period, std::function<void()> callback);
  void every(uint32_t period, std::function<void()> callback, int16_t repeatCount);
  void after(uint32_t duration, std::function<void()> callback);

  void update();
  void update(uint32_t now);

  void run() override;

private:
  int8_t findFreeEventIndex();

private:
  Timer _events[maxNumberOfEvents];
  uint32_t _nAt;
};

};
#endif
