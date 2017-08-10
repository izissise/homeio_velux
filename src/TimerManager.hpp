#ifndef _TIMERMANAGER_HPP_
#define _TIMERMANAGER_HPP_

#include <cstdint>
#include <functional>

constexpr static uint32_t nowMaxValue = 0xffffffff;
constexpr static int8_t maxNumberOfEvents = 4;

class TimerManager {
public:

  TimerManager();
  ~TimerManager() = default;

  void every(uint32_t period, std::function<void()> callback);
  void every(uint32_t period, std::function<void()> callback, int16_t repeatCount);
  void after(uint32_t duration, std::function<void()> callback);

  void update();
  void update(uint32_t now);

public:
  struct timer {
    uint32_t period;
    uint32_t lastTime;
    int16_t repeatCount;
    std::function<void()> func;
  };

private:
  int8_t findFreeEventIndex();

private:
  timer _events[maxNumberOfEvents];
  uint32_t _nAt;
};

#endif
