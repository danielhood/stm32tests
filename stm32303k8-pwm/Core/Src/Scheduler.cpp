#include "Scheduler.h"

Scheduler::Scheduler()
  : _count(0)
{
  for (int i=0;i<8;i++) _faders[i] = nullptr;
}

Scheduler::~Scheduler()
{
}

Scheduler& Scheduler::instance()
{
  static Scheduler s;
  return s;
}

void Scheduler::registerFader(PWMFader* fader)
{
  if (!fader) return;
  for (int i=0;i<_count;i++) if (_faders[i]==fader) return;
  if (_count < 8) _faders[_count++] = fader;
}

void Scheduler::unregisterFader(PWMFader* fader)
{
  if (!fader) return;
  for (int i=0;i<_count;i++) {
    if (_faders[i]==fader) {
      for (int j=i;j<_count-1;j++) _faders[j] = _faders[j+1];
      _faders[_count-1] = nullptr;
      _count--;
      return;
    }
  }
}

void Scheduler::tick(uint32_t ms)
{
  for (int i=0;i<_count;i++) {
    if (_faders[i]) _faders[i]->tick(ms);
  }
}
