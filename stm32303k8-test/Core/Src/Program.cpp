/*
 * Program.cpp
 *
 *  Created on: Mar 13, 2024
 *      Author: Daniel Hood
 */

#include "Program.h"
#include "Tests.h"

Program::Program(UART_HandleTypeDef* hUart)
	: _log(hUart) {
	_shouldEnd = false;
}

Program::~Program() {
}

void Program::Run(UART_HandleTypeDef* hUart) {
	Program program(hUart);

	program.Run();
}

void Program::Run() {
	_log.Debug("Program -- Start\r\n");

	int iteration = 0;

	while (!_shouldEnd){
		Loop(iteration++);
	}

	_log.Debug("Program -- End\r\n");
}

void Program::Loop(int iteration) {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

	_log.Debug("Loop#%i\r\n", iteration);

	// Read PA12 (D2)
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);

	Tests tests(1);
	int testResult = tests.ExecuteTests();

	if (pinState == GPIO_PIN_SET) {
		HAL_Delay(testResult);
	}
	else {
		HAL_Delay(testResult * 4);
	}

	_shouldEnd = iteration > 50;
}
