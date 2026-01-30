#include "TIM2Handler.h"
#include "Scheduler.h"

TIM2Handler* TIM2Handler::_instance = nullptr;

TIM2Handler::TIM2Handler()
  : _htim(nullptr)
{
  _instance = this;
  _statusReload = 100;
  _statusCount = _statusReload;
}

TIM2Handler::TIM2Handler(TIM_HandleTypeDef* htim)
  : _htim(htim)
{
  _instance = this;
  _statusReload = 100;
  _statusCount = _statusReload;
}

TIM2Handler::~TIM2Handler()
{
  if (_instance == this) _instance = nullptr;
}

void TIM2Handler::begin(TIM_HandleTypeDef* htim)
{
  _htim = htim;
  _instance = this;
  _statusReload = 100;
  _statusCount = _statusReload;
}

void TIM2Handler::end()
{
  if (_instance == this) _instance = nullptr;
  _htim = nullptr;
}

void TIM2Handler::handlePeriodElapsed(TIM_HandleTypeDef* htim)
{
  if (!_htim) return;
  if (htim->Instance == _htim->Instance) {
    /* Drive scheduler tick (1 ms by default) */
    Scheduler::instance().tick(1);

    /* decrement and check using reload to avoid underflow */
    if (_statusCount > 0) {
      if (--_statusCount == 0) {
        _statusCount = _statusReload;
        /* Toggle LED on PB3 */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
      }
    }
  }
}

TIM2Handler* TIM2Handler::instance()
{
  return _instance;
}

void TIM2Handler::setStatusInterval(uint32_t ms)
{
  _statusReload = (ms > 0) ? ms : 1;
  _statusCount = _statusReload;
}

uint32_t TIM2Handler::getStatusInterval() const
{
  return _statusReload;
}

extern "C" {

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (TIM2Handler::instance()) {
    TIM2Handler::instance()->handlePeriodElapsed(htim);
  }
}

} // extern "C"
