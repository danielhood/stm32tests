/*
 * Log.cpp
 *
 *  Created on: Mar 13, 2024
 *      Author: Daniel Hood
 */

#include "Log.h"
#include <cstring>

Log::Log(UART_HandleTypeDef* hUart) {
	_hUart = hUart;
}

Log::~Log() {
}

void Log::Debug(const std::string format, ...) {
	if (NULL == _hUart) return;
	if (format.empty()) return;

	va_list app;
	va_start(app, format);
	std::string msg = string_format(format, app);
	va_end(app);

	HAL_UART_Transmit(_hUart, (uint8_t *)msg.c_str(), msg.length(), 0xFFFF);
}

std::string Log::string_format(const std::string format, va_list ap) {
    int final_n, n = ((int)format.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::unique_ptr<char[]> formatted;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], format.c_str());
        final_n = vsnprintf(&formatted[0], n, format.c_str(), ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}
