/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_mfrc522_interface_template.c
 * @brief     driver mfrc522 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-05-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/05/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mfrc522_interface.h"
#include "stm32l4xx_hal.h"
#include <stdarg.h>

extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi1;

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 reset gpio init failed
 * @note   none
 */
uint8_t mfrc522_interface_reset_gpio_init(void)
{
  mfrc522_interface_debug_print("mfrc522_interface_reset_gpio_init\r\n");
    return 0;
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 reset gpio deinit failed
 * @note   none
 */
uint8_t mfrc522_interface_reset_gpio_deinit(void)
{
  mfrc522_interface_debug_print("mfrc522_interface_reset_gpio_deinit\r\n");
    return 0;
}

/**
 * @brief     interface reset gpio write
 * @param[in] value written value
 * @return    status code
 *            - 0 success
 *            - 1 reset gpio write failed
 * @note      none
 */
uint8_t mfrc522_interface_reset_gpio_write(uint8_t value)
{
  mfrc522_interface_debug_print("mfrc522_interface_reset_gpio_write: %d\r\n", value);
  if (value != 0)
  {
      /* set high */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
  }
  else
  {
      /* set low */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
  }

    return 0;
}

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t mfrc522_interface_iic_init(void)
{
    return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t mfrc522_interface_iic_deinit(void)
{
    return 0;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr iic device write address
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mfrc522_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mfrc522_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t mfrc522_interface_spi_init(void)
{
  mfrc522_interface_debug_print("mfrc522_interface_spi_init\r\n");
    return 0;
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t mfrc522_interface_spi_deinit(void)
{
  mfrc522_interface_debug_print("mfrc522_interface_spi_deinit\r\n");
    return 0;
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mfrc522_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
  mfrc522_interface_debug_print("mfrc522_interface_spi_read: reg=%d len=%d\r\n", reg, len);

  uint8_t buffer;
  uint8_t res;

  /* set cs low */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /* transmit the addr */
  buffer = reg;
  res = HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer, 1, 1000);
  if (res != HAL_OK)
  {
      /* set cs high */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

      return 1;
  }

  /* if len > 0 */
  if (len > 0)
  {
      /* receive to the buffer */
      res = HAL_SPI_Receive(&hspi1, buf, len, 1000);
      if (res != HAL_OK)
      {
          /* set cs high */
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

          return 1;
      }
  }

  /* set cs high */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  mfrc522_interface_debug_print("read buffer: ");
  mfrc522_interface_debug_print_hex(buf, len);

    return 0;
}

/**
 * @brief     interface spi bus write
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mfrc522_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
  mfrc522_interface_debug_print("mfrc522_interface_spi_write: reg=%d buf=", reg);
  mfrc522_interface_debug_print_hex(buf, len);

  uint8_t buffer;
  uint8_t res;

  /* set cs low */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /* transmit the addr */
  buffer = reg;
  res = HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer, 1, 1000);
  if (res != HAL_OK)
  {
      /* set cs high */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

      return 1;
  }

  /* if len > 0 */
  if (len > 0)
  {
      /* transmit the buffer */
      res = HAL_SPI_Transmit(&hspi1, buf, len, 1000);
      if (res != HAL_OK)
      {
          /* set cs high */
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

          return 1;
      }
  }

  /* set cs high */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief  interface uart init
 * @return status code
 *         - 0 success
 *         - 1 uart init failed
 * @note   none
 */
uint8_t mfrc522_interface_uart_init(void)
{
    return 0;
}

/**
 * @brief  interface uart deinit
 * @return status code
 *         - 0 success
 *         - 1 uart deinit failed
 * @note   none
 */
uint8_t mfrc522_interface_uart_deinit(void)
{
    return 0;
}

/**
 * @brief      interface uart read
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint16_t mfrc522_interface_uart_read(uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief     interface uart write
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mfrc522_interface_uart_write(uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief  interface uart flush
 * @return status code
 *         - 0 success
 *         - 1 uart flush failed
 * @note   none
 */
uint8_t mfrc522_interface_uart_flush(void)
{
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void mfrc522_interface_delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}

/**
 * @brief     print buffer bytes in hex via debug print
 * @param[in] buf buffer to print
 * @param[in] len number of bytes
 * @note      none
 */
void mfrc522_interface_debug_print_hex(const uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        mfrc522_interface_debug_print("0x%02X ", buf[i]);
    }
    mfrc522_interface_debug_print("\r\n");
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void mfrc522_interface_debug_print(const char *const fmt, ...)
{
#ifndef NO_DEBUG
    char str[256];
    uint16_t len;
    va_list args;

    memset((char *)str, 0, sizeof(char) * 256);
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);

    len = strlen((char *)str);

    HAL_UART_Transmit(&huart2, (uint8_t*)str, len, HAL_MAX_DELAY);
#endif

}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
void mfrc522_interface_receive_callback(uint16_t type)
{
    switch (type)
    {
        case MFRC522_INTERRUPT_MFIN_ACT :
        {
            mfrc522_interface_debug_print("mfrc522: irq mfin act.\n");

            break;
        }
        case MFRC522_INTERRUPT_CRC :
        {
            mfrc522_interface_debug_print("mfrc522: irq crc.\n");

            break;
        }
        case MFRC522_INTERRUPT_TX :
        {
            mfrc522_interface_debug_print("mfrc522: irq tx.\n");

            break;
        }
        case MFRC522_INTERRUPT_RX :
        {
            mfrc522_interface_debug_print("mfrc522: irq rx.\n");

            break;
        }
        case MFRC522_INTERRUPT_IDLE :
        {
            mfrc522_interface_debug_print("mfrc522: irq idle.\n");

            break;
        }
        case MFRC522_INTERRUPT_HI_ALERT :
        {
            mfrc522_interface_debug_print("mfrc522: irq hi alert.\n");

            break;
        }
        case MFRC522_INTERRUPT_LO_ALERT :
        {
            mfrc522_interface_debug_print("mfrc522: irq lo alert.\n");

            break;
        }
        case MFRC522_INTERRUPT_ERR :
        {
            mfrc522_interface_debug_print("mfrc522: irq err.\n");

            break;
        }
        case MFRC522_INTERRUPT_TIMER :
        {
            mfrc522_interface_debug_print("mfrc522: irq timer.\n");

            break;
        }
        default :
        {
            mfrc522_interface_debug_print("mfrc522: irq unknown code.\n");

            break;
        }
    }
}
