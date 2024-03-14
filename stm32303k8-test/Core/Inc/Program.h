/*
 * Program.h
 *
 *  Created on: Mar 13, 2024
 *      Author: Daniel Hood
 */

#ifndef SRC_PROGRAM_H_
#define SRC_PROGRAM_H_

#include "main.h"
#include "Log.h"

class Program {
public:

	virtual ~Program();

	static void Run(UART_HandleTypeDef* hUart);

private:
	Log _log;
	bool _shouldEnd;

	Program(UART_HandleTypeDef* hUart);
	void Run();

	void Loop(int iteration);
};

#endif /* SRC_PROGRAM_H_ */
