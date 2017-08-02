#ifndef IJOB_HPP
#define IJOB_HPP

#include "TimerManager.hpp"

class IJob {
public:
  virtual ~IJob() = default;

  virtual void run(TimerManager& tm) = 0;
};

#endif // IJOB_HPP
