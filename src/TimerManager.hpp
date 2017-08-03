#ifndef _TIMERMANAGER_HPP_
#define _TIMERMANAGER_HPP_

#include <cstdint>
#include <functional>

constexpr static int8_t maxNumberOfEvents = 10;

class TimerManager {
public:

  TimerManager();
  ~TimerManager() = default;

  void every(uint32_t period, void (*callback)(void*), void* arg);
  void every(uint32_t period, void (*callback)(void*), void* arg, int16_t repeatCount);
  void after(uint32_t duration, void (*callback)(void*), void* arg);

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
  timer _events[maxNumberOfEvents];
  int8_t findFreeEventIndex();
};

#endif
