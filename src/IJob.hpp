#ifndef IJOB_HPP
#define IJOB_HPP

#include "TimerManager.hpp"

class IJob {
public:
  virtual ~IJob() = default;

  virtual void passTimeManager(TimerManager& tm) = 0;
  virtual void run() = 0;
};

#endif // IJOB_HPP
