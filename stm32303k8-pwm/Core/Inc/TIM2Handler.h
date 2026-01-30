#ifndef TIM2_HANDLER_H
#define TIM2_HANDLER_H

#include "main.h"

class TIM2Handler {
public:
  TIM2Handler();
  explicit TIM2Handler(TIM_HandleTypeDef* htim);
  ~TIM2Handler();

  void begin(TIM_HandleTypeDef* htim);
  void end();

  void handlePeriodElapsed(TIM_HandleTypeDef* htim);

  static TIM2Handler* instance();

private:
  TIM_HandleTypeDef* _htim;
  static TIM2Handler* _instance;

  volatile uint32_t _statusCount;
  uint32_t _statusReload;

public:
  /**
   * Set the LED toggle interval in milliseconds (default 1000 ms).
   */
  void setStatusInterval(uint32_t ms);
  uint32_t getStatusInterval() const;
};

#endif // TIM2_HANDLER_H
