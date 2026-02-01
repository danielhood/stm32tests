# stm32tests

STM32F303K8 test and example projects for PWM control and embedded debugging.

## Projects

### stm32f303k8-i2c
I2C connectivity example using STM32F303K8 I2C 1. Demonstrates two-way communication on the I2C bus with an external module.

**Configuration**
- I2C1 configured on:
  - PB7 (SDA)
  - PB6 (SCL)
- Status LED:
  - PB3 (GPIO OUT)

### stm32f303k8-pwm
PWM fade control example using STM32F303K8 Timer 1. Demonstrates smooth LED fading using PWM duty cycle modulation.

**Features:**
- Timer 1 PWM on PA8 (CH1) with an LED
- C++ `PWMFader` class for encapsulated fade control
- 4 PWM channels configured:
  - Channels 1-2: 2 second fade cycle
  - Channels 3-4: 200ms fade cycle
- PB3 GPIO toggle for debug LED

**Key Files:**
- `Core/Src/PWMFader.cpp` - PWM fading implementation
- `Core/Inc/PWMFader.h` - PWM fader class definition
- `Core/Src/main.c` - Main application

### stm32f303k8-test
Embedded test and debug program with UART serial logging capabilities.

**Features:**
- Serial debug logging via UART2 (38400 baud)
- `Log.cpp` class for formatted string output
- Program loop with configurable iterations
- GPIO input reading for test control
- `Tests` class for executing test code

## Building
Build using STM32CubeIDE

## UART Serial Debug Logging
In the stm32f303k8-test project, a `Log.cpp` class sends formatted string data to the UART2 port through `HAL_UART_Transmit()`.

UART2 must be enabled through the .ioc file in async mode. This generates pin configuration in main.c which should be manually mapped to main.cpp (the IDE generates to main.c by default).

**To view serial data:**
1. Open Command Shell Console from the Console tool window
2. Create a mapping for the COM port (e.g. COM3)
3. Set parameters: BaudRate = 38400, Stop bits = 1, Parity = none

Serial output streams in both debug and run modes.

## IOC Pin Code Generation
When the .ioc file is updated with pin configuration changes, STM32CubeIDE only generates code to main.c. For C++ projects:

1. Copy entire main.c to main.cpp
2. User code sections are preserved
3. Project compiles as C++

Program logic is separated and called through `Program::Run()`.
