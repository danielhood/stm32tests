#include "PWMFader.h"

PWMFader::PWMFader(TIM_HandleTypeDef *timer, uint32_t ch, uint32_t periodMs)
  : htim(timer), channel(ch), cyclePeriod(periodMs), 
    fadeTimer(0), fadePulse(0), maxPulse(65535), isRunning(false)
{
}

PWMFader::~PWMFader()
{
  stop();
}

void PWMFader::start(void)
{
  if (!isRunning)
  {
    HAL_TIM_PWM_Start(htim, channel);
    isRunning = true;
    fadeTimer = 0;
    fadePulse = 0;
  }
}

void PWMFader::stop(void)
{
  if (isRunning)
  {
    HAL_TIM_PWM_Stop(htim, channel);
    isRunning = false;
  }
}

void PWMFader::setPeriod(uint32_t periodMs)
{
  cyclePeriod = periodMs;
  fadeTimer = 0;
  fadePulse = 0;
}

void PWMFader::update(void)
{
  // kept for compatibility: call tick(1)
  tick(1);
}

void PWMFader::tick(uint32_t ms)
{
  if (!isRunning)
    return;

  uint32_t halfPeriod = cyclePeriod / 2;

  /* advance timer by ms milliseconds */
  fadeTimer += ms;

  if (fadeTimer < halfPeriod)
  {
    /* Fade in: increase pulse from 0 to maxPulse over halfPeriod ms */
    fadePulse = (fadeTimer * maxPulse) / halfPeriod;
  }
  else if (fadeTimer < cyclePeriod)
  {
    /* Fade out: decrease pulse from maxPulse to 0 over halfPeriod ms */
    fadePulse = ((cyclePeriod - fadeTimer) * maxPulse) / halfPeriod;
  }
  else
  {
    /* Reset timer for next cycle */
    fadeTimer = 0;
    fadePulse = 0;
  }

  /* Update PWM pulse width */
  __HAL_TIM_SET_COMPARE(htim, channel, fadePulse);
}
