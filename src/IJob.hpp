#ifndef IJOB_HPP
#define IJOB_HPP

class IJob {
public:
  virtual ~IJob() = default;

  virtual void run() = 0;
};

#endif // IJOB_HPP
