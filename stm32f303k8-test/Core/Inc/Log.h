/*
 * Log.h
 *
 *  Created on: Mar 13, 2024
 *      Author: Daniel Hood
 */

#ifndef SRC_LOG_H_
#define SRC_LOG_H_

#include "main.h"

#include <memory>
#include <string>
#include <stdarg.h>

class Log {
public:
	Log(UART_HandleTypeDef* hUart);
	virtual ~Log();

	void Debug(const std::string format, ...);

private:
	UART_HandleTypeDef* _hUart;
	std::string string_format(const std::string format, va_list ap);
};

#endif /* SRC_LOG_H_ */
