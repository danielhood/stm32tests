# stm32tests

## Building
Build using STM32CubeIDE

## UART Serial Debug Logging
In the stm23tests, created a Log.cpp class that sends formatted string data to the UART2 port through `HAL_UART_Transmit()`

UART2 needs to be enabled through the .ioc file, in async mode. This will generate pin configuration and init code in main.c which needs to be manually mapped over to the main.cpp file (unsure how to have it generate directly to the cpp file).

To view the serial data, in the IDE, you need to open a Command Shell Console from the main Console tool window, create a mapping for the current com port (e.g. COM3) and set the com parameters the same as what is configured in the code (BaudRate = 38400, Stop bits = 1, parity = none).

The serial will stream in both debug and run modes.

## IOC Pin Code Generation
Every time the .ioc file is updated with pin config changes, the IDE only generates the code config to main.c. Since this is a cpp project, the entire main.c file should be copied into main.cpp.

The user code will be preserved, and this will still compile as a cpp project.

The program logic is separated and called through Program:Run().
