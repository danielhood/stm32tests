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

	Tests tests(1);
	HAL_Delay(tests.ExecuteTests());

	_shouldEnd = iteration > 50;
}
