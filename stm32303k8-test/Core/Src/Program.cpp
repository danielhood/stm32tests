/*
 * Program.cpp
 *
 *  Created on: Mar 13, 2024
 *      Author: conflictedoperator
 */

#include "Program.h"
#include "Tests.h"
Program::Program() {
	// TODO Auto-generated constructor stub

}

Program::~Program() {
	// TODO Auto-generated destructor stub
}

void Program::Run(UART_HandleTypeDef* huart2) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

		HAL_UART_Transmit(huart2, (uint8_t *)"x", 1, 0xFFFF);
		HAL_UART_Transmit(huart2, (uint8_t *)"\r", 1, 0xFFFF);
		HAL_UART_Transmit(huart2, (uint8_t *)"\n", 1, 0xFFFF);
		Tests* tests = new Tests(1);
		HAL_Delay(tests->ExecuteTests());
		//HAL_Delay(100);
}
