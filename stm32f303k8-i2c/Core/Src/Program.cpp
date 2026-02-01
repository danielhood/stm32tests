#include "Program.h"
#include <cstring>

Program::Program(UART_HandleTypeDef* hUart, I2C_HandleTypeDef* hI2c) 
  : _log(hUart), _i2c(hI2c) {
  
}

Program::~Program() {

}

void Program::Run() {
  _log.Debug("Launched as I2C slave 0x60!\r\n");
  
  while (1) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    
    HAL_Delay(500);
  }
}

void Program::OnI2CReceiveEnabled(){
	_log.Debug("I2C Rx Enabled");
}

void Program::OnI2CReceived(uint8_t* data, uint16_t len) {
  /* Log received data */
  if (len > 0) {
    _log.Debug("I2C Rx (%u bytes):\r\n", len);
  }

  /* Prepare response and send it back to master */
  const char response[] = "STM32-response";
  HAL_StatusTypeDef txr = HAL_I2C_Slave_Transmit_IT(_i2c, (uint8_t*)response, (uint16_t)strlen(response));
  if (txr == HAL_OK) {
    _log.Debug("I2C Tx response OK\r\n");
  } else {
    _log.Debug("I2C Tx response ERR: %d\r\n", (int)txr);
  }

  /* Re-enable listening for next master */
  HAL_I2C_EnableListen_IT(_i2c);
}
