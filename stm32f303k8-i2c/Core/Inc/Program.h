#ifndef PROGRAM_H
#define PROGRAM_H

#include "main.h"
#include "Log.h"

class Program {
public:
  Program(UART_HandleTypeDef* hUart, I2C_HandleTypeDef* hI2c);
  ~Program();

  void Run();
  void OnI2CReceived(uint8_t* data, uint16_t len);
  void OnI2CReceiveEnabled();
  
  uint8_t* GetI2CRxBuffer() { return _i2cRxBuf; }
  uint8_t* GetI2CTxBuffer() { return _i2cTxBuf; }

  static const uint16_t I2C_RX_BUF_SIZE = 128;
  static const uint16_t I2C_TX_BUF_SIZE = 128;

private:
  Log _log;
  I2C_HandleTypeDef* _i2c;
  uint8_t _i2cRxBuf[I2C_RX_BUF_SIZE];
  uint8_t _i2cTxBuf[I2C_TX_BUF_SIZE];
  
};

#endif // PROGRAM_H
