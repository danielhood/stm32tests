#ifndef PWM_FADER_H
#define PWM_FADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#ifdef __cplusplus
}
#endif

class PWMFader
{
public:
  PWMFader(TIM_HandleTypeDef *timer, uint32_t channel, uint32_t cyclePeriod = 2000);
  ~PWMFader();
  
  void update(void);
  void start(void);
  void stop(void);
  void setPeriod(uint32_t periodMs);
  
private:
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  uint32_t cyclePeriod;  // Total cycle period in milliseconds (up + down)
  uint32_t fadeTimer;
  uint32_t fadePulse;
  uint32_t maxPulse;
  bool isRunning;
};

#endif // PWM_FADER_H
