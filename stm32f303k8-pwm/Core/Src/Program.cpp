#include "Program.h"
#include "PWMFader.h"
#include "TIM2Handler.h"
#include "Scheduler.h"

Program::Program(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2)
  : _htim1(htim1), _htim2(htim2),
    _f1(nullptr), _f2(nullptr), _f3(nullptr), _f4(nullptr), _tim2Handler(nullptr)
{
  if (_htim2) {
    _tim2Handler = new TIM2Handler(_htim2);
    /* override default status interval to 500 ms */
    _tim2Handler->setStatusInterval(500);
    HAL_TIM_Base_Start_IT(_htim2);
  }

  if (_htim1) {
    _f1 = new PWMFader(_htim1, TIM_CHANNEL_1, 1900);
    _f2 = new PWMFader(_htim1, TIM_CHANNEL_2, 1100);
    _f3 = new PWMFader(_htim1, TIM_CHANNEL_3, 500);
    _f4 = new PWMFader(_htim1, TIM_CHANNEL_4, 300);

    _f1->start();
    _f2->start();
    _f3->start();
    _f4->start();
    
    /* Register faders with scheduler for hardware-timed ticks */
    Scheduler::instance().registerFader(_f1);
    Scheduler::instance().registerFader(_f2);
    Scheduler::instance().registerFader(_f3);
    Scheduler::instance().registerFader(_f4);
  }
}

Program::~Program() {
  /* Unregister faders before deletion */
  Scheduler::instance().unregisterFader(_f1);
  Scheduler::instance().unregisterFader(_f2);
  Scheduler::instance().unregisterFader(_f3);
  Scheduler::instance().unregisterFader(_f4);

  if (_f1) { _f1->stop(); delete _f1; }
  if (_f2) { _f2->stop(); delete _f2; }
  if (_f3) { _f3->stop(); delete _f3; }
  if (_f4) { _f4->stop(); delete _f4; }

  if (_tim2Handler) {
    HAL_TIM_Base_Stop_IT(_htim2);
    delete _tim2Handler;
  }
}

void Program::Run() {
  /* Main loop can be idle; faders are advanced by hardware timer via Scheduler */
  while (1) {
    HAL_Delay(100);
  }
}
