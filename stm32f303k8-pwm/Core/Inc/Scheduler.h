#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "main.h"
#include "PWMFader.h"

class Scheduler {
public:
  static Scheduler& instance();
  void registerFader(PWMFader* fader);
  void unregisterFader(PWMFader* fader);
  void tick(uint32_t ms = 1);

private:
  Scheduler();
  ~Scheduler();
  PWMFader* _faders[8];
  int _count;
};

#endif // SCHEDULER_H
