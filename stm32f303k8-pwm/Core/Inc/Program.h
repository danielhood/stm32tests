#ifndef PROGRAM_H
#define PROGRAM_H

#include "main.h"
#include "PWMFader.h"
#include "TIM2Handler.h"

class Program {
public:
  Program(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2);
  ~Program();

  void Run();

private:
  TIM_HandleTypeDef* _htim1;
  TIM_HandleTypeDef* _htim2;
  PWMFader* _f1;
  PWMFader* _f2;
  PWMFader* _f3;
  PWMFader* _f4;
  TIM2Handler* _tim2Handler;
};

#endif // PROGRAM_H
