#ifndef _TIMERMANAGER_HPP_
#define _TIMERMANAGER_HPP_

#include <cstdint>
#include <functional>

#include "IJob.hpp"

namespace Timer {

constexpr uint32_t nowMaxValue = 0xffffffff;
constexpr int8_t maxNumberOfEvents = 2;

class Timer {
public:
  Timer() = default;
  explicit Timer(uint32_t period, int16_t repeatCount = 1);

  ~Timer() = default;

  bool update();
  bool update(uint32_t now);

  bool waitNextUpdate();
  uint32_t nextTriggerus(uint32_t from) const { return _period - (from % _period); }
  int16_t repeatLeft() const { return _repeatCount; }

protected:
  uint32_t _period = 0;
  uint32_t _lastTime = 0;
  int16_t  _repeatCount = 0;
};

Timer every(uint32_t period);
Timer every(uint32_t period, int16_t repeatCount);
Timer after(uint32_t duration);

class TimerManager : public IJob {
public:
  TimerManager();
  virtual ~TimerManager() = default;

  void attachTimer(Timer timer, std::function<void()> callback);

  void run() override;

private:
  void update(uint32_t now);
  void update();
  int8_t findFreeEventIndex();

private:
  struct TimerBoundFunc {
    Timer timer;
    std::function<void()> cb;
  };

private:
  TimerBoundFunc _events[maxNumberOfEvents];
  uint32_t _nAt;
};

};
#endif
