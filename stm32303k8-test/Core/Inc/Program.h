/*
 * Program.h
 *
 *  Created on: Mar 13, 2024
 *      Author: conflictedoperator
 */

#ifndef SRC_PROGRAM_H_
#define SRC_PROGRAM_H_

#include "main.h"

class Program {
public:
	Program();
	virtual ~Program();

	static void Run(UART_HandleTypeDef* huart2);
};

#endif /* SRC_PROGRAM_H_ */
