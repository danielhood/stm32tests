/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdarg.h>
#include "driver_mfrc522_basic.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MSG_MAX 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

char msg[MSG_MAX];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void main_debug_print(const char *const fmt, ...)
{
#ifndef NO_DEBUG_MAIN
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

void main_debug_print_hex(const uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        main_debug_print("0x%02X ", buf[i]);
    }
}


uint16_t calcCRC(uint8_t* buf, uint16_t len)
{
  // Calc CRC bytes (note this can be done on the rc522)
  uint16_t crc = 0x6363; // Preset value for ISO 14443-3
  for (uint8_t i = 0; i < len; i++) {
      uint8_t ch = buf[i] ^ (uint8_t)(crc & 0x00FF);
      ch = ch ^ (ch << 4);
      crc = (crc >> 8) ^ ((uint16_t)ch << 8) ^ ((uint16_t)ch << 3) ^ (ch >> 4);
  }

  buf[len] = (uint8_t)(crc & 0xFF);        // CRC Low byte
  buf[len+1] = (uint8_t)((crc >> 8) & 0xFF); // CRC High byte

  return crc;
}

static void a_callback(uint16_t type)
{
    switch (type)
    {
        case MFRC522_INTERRUPT_MFIN_ACT :
        {
            main_debug_print("mfrc522: irq mfin act.\n");

            break;
        }
        case MFRC522_INTERRUPT_CRC :
        {
            break;
        }
        case MFRC522_INTERRUPT_TX :
        {
            break;
        }
        case MFRC522_INTERRUPT_RX :
        {
            break;
        }
        case MFRC522_INTERRUPT_IDLE :
        {
            break;
        }
        case MFRC522_INTERRUPT_HI_ALERT :
        {
            break;
        }
        case MFRC522_INTERRUPT_LO_ALERT :
        {
            break;
        }
        case MFRC522_INTERRUPT_ERR :
        {
            main_debug_print("mfrc522: irq err.\n");

            break;
        }
        case MFRC522_INTERRUPT_TIMER :
        {
            main_debug_print("mfrc522: irq timer.\n");

            break;
        }
        default :
        {
            main_debug_print("mfrc522: irq unknown code.\n");

            break;
        }
    }
}

void getInfo(void)
{
  mfrc522_info_t info;

  /* print mfrc522 info */
  mfrc522_info(&info);
  main_debug_print("mfrc522: chip is %s.\n", info.chip_name);
  main_debug_print("mfrc522: manufacturer is %s.\n", info.manufacturer_name);
  main_debug_print("mfrc522: interface is %s.\n", info.interface);
  main_debug_print("mfrc522: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
  main_debug_print("mfrc522: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
  main_debug_print("mfrc522: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
  main_debug_print("mfrc522: max current is %0.2fmA.\n", info.max_current_ma);
  main_debug_print("mfrc522: max temperature is %0.1fC.\n", info.temperature_max);
  main_debug_print("mfrc522: min temperature is %0.1fC.\n", info.temperature_min);
}


uint8_t initMfrc522()
{
  uint8_t addr = 0x00;

  main_debug_print("main: Initializing mfrc522 as SPI...\r\n");

  /* basic int */
  uint8_t res = mfrc522_basic_init(MFRC522_INTERFACE_SPI, addr, a_callback);
  if (res != 0)
  {
      main_debug_print("main: mfrc522_basic_init failed.\r\n");
      return 1;
  }

  return 0;
}

uint8_t getRandom(void)
{
  uint8_t res;
  uint8_t buf[25];

  main_debug_print("main: Getting random generated id...\r\n");

  /* get the random */
  memset(buf, 0, sizeof(buf));
  res = mfrc522_basic_generate_random(buf);
  if (res != 0)
  {
      main_debug_print("main: mfrc522_basic_generate_random failed.\r\n");
      return 1;
  }

  /* output */
  main_debug_print("main: Received random ID:\r\n\t");
  main_debug_print_hex(buf, 25);
  main_debug_print("\r\n");

  return 0;
}

void getVersion(void)
{
	uint8_t res;
	uint8_t id = 0;
	uint8_t version = 0;

  main_debug_print("main: Getting version..\r\n");

  res = mfrc522_get_vesion(&id, &version);
  if (res != 0)
  {
      main_debug_print("main: mfrc522_get_vesion failed.\r\n");
      return;
  }

  main_debug_print("id: %d  version: %d\r\n", id, version);

}

uint8_t readATQA()
{
  uint8_t res;
  uint8_t command = 0x26; // receive
  uint8_t buf[64];
  uint8_t out_len;

  main_debug_print("main: Reading ATQA...\r\n");

  memset(buf, 0, sizeof(buf));
  out_len = sizeof(buf);
  res = mfrc522_basic_transceiver(&command, 1, buf, &out_len); // out_len will be reduced if less data is returned; will not return more than initial value of out_len
  if (res != 0 || out_len == 0)
  {
      main_debug_print("main: mfrc522_basic_transceiver failed.\r\n");
      return 1;
  }

  main_debug_print("main: Received %d bytes of ATQA data: ", out_len);
  main_debug_print_hex(buf, out_len);
  main_debug_print("\r\n");

  return 0;

}


uint8_t readID(void)
{
  uint8_t res;
  uint8_t commandCL1[] = { 0x93, 0x20 };  // mifare anti-coll
  uint8_t commandCL1ACK[] = { 0x93, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00 };  // mifare anti-coll
  uint8_t commandCL2[] = { 0x95, 0x20 };  // mifare anti-col2
  uint8_t commandCL2ACK[] = { 0x95, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00 };  // mifare anti-coll
  uint8_t buf[64];
  uint8_t out_len;

  uint8_t fullId[7];

  if (0 != readATQA()) {
    return 1;
  }

  main_debug_print("main: Reading ID (CL1)...\r\n");

  memset(buf, 0, sizeof(buf));
  out_len = sizeof(buf);
  res = mfrc522_basic_transceiver(commandCL1, 2, buf, &out_len); // out_len will be reduced if less data is returned; will not return more than initial value of out_len
  if (res != 0 || out_len == 0)
  {
      main_debug_print("main: mfrc522_basic_transceiver failed.\r\n");
      return 1;
  }

  main_debug_print("main: Received %d bytes of ID (CL1) data: ", out_len);
  main_debug_print_hex(buf, out_len);
  main_debug_print("\r\n");

  // Copy first 3 bytes of ID
  fullId[0] = buf[1];
  fullId[1] = buf[2];
  fullId[2] = buf[3];

  main_debug_print("main: ACK ID (CL1)...\r\n");

  commandCL1ACK[2] = buf[0];
  commandCL1ACK[3] = buf[1];
  commandCL1ACK[4] = buf[2];
  commandCL1ACK[5] = buf[3];
  commandCL1ACK[6] = buf[4];

  calcCRC(commandCL1ACK, 7);

  memset(buf, 0, sizeof(buf));
  out_len = sizeof(buf);

  res = mfrc522_basic_transceiver(commandCL1ACK, 9, buf, &out_len); // out_len will be reduced if less data is returned; will not return more than initial value of out_len
  if (res != 0 || out_len == 0)
  {
      main_debug_print("main: mfrc522_basic_transceiver failed.\r\n");
      return 1;
  }

  main_debug_print("main: Received %d bytes of ACK ID (CL1) data: ", out_len);
  main_debug_print_hex(buf, out_len);
  main_debug_print("\r\n");

  main_debug_print("main: Reading ID (CL2)...\r\n");

  memset(buf, 0, sizeof(buf));
  out_len = sizeof(buf);
  res = mfrc522_basic_transceiver(commandCL2, 2, buf, &out_len); // out_len will be reduced if less data is returned; will not return more than initial value of out_len
  if (res != 0 || out_len == 0)
  {
      main_debug_print("main: mfrc522_basic_transceiver failed.\r\n");
      return 1;
  }

  main_debug_print("main: Received %d bytes of ID (CL2) data: ", out_len);
  main_debug_print_hex(buf, out_len);
  main_debug_print("\r\n");

  // Copy remaining 5 bytes of ID
  fullId[3] = buf[0];
  fullId[4] = buf[1];
  fullId[5] = buf[2];
  fullId[6] = buf[3];


  main_debug_print("main: ACK ID (CL2)...\r\n");

  commandCL2ACK[2] = buf[0];
  commandCL2ACK[3] = buf[1];
  commandCL2ACK[4] = buf[2];
  commandCL2ACK[5] = buf[3];
  commandCL2ACK[6] = buf[4];

  calcCRC(commandCL2ACK, 7);

  memset(buf, 0, sizeof(buf));
  out_len = sizeof(buf);

  res = mfrc522_basic_transceiver(commandCL2ACK, 9, buf, &out_len); // out_len will be reduced if less data is returned; will not return more than initial value of out_len
  if (res != 0 || out_len == 0)
  {
      main_debug_print("main: mfrc522_basic_transceiver failed.\r\n");
      return 1;
  }

  main_debug_print("main: Received %d bytes of ACK ID (CL1) data: ", out_len);
  main_debug_print_hex(buf, out_len);
  main_debug_print("\r\n");

  main_debug_print("main: Full ID: ");
  main_debug_print_hex(fullId, sizeof(fullId));
  main_debug_print("\r\n\r\n\r\n");

  return 0;
}

uint8_t readNTAG(void)
{
  uint8_t res;
  uint8_t commandNTAG[] = { 0x30, 0x04, 0x00, 0x00 };  // PICC_CMD_MF_READ, page 4, plus crc
  uint8_t buf[64];
  uint8_t out_len;

  // In order to read NTAG, the chip must query ATQA followed by full id
  if (0 != readID())
  {
    return 1;
  }

  main_debug_print("main: Reading NTAG...\r\n");

  calcCRC(commandNTAG, 2);

  memset(buf, 0, sizeof(buf));
  out_len = sizeof(buf);
  res = mfrc522_basic_transceiver(commandNTAG, 4, buf, &out_len); // out_len will be reduced if less data is returned; will not return more than initial value of out_len
  if (res != 0 || out_len == 0)
  {
      main_debug_print("main: mfrc522_basic_transceiver failed.\r\n");
      return 1;
  }

  main_debug_print("main: Received %d bytes of NTAG data: ", out_len);
  main_debug_print_hex(buf, out_len);
  main_debug_print("\r\n\r\n\r\n");

  // Note: custom text tag starts at byte 10 of page 4

  return 0;

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  snprintf(msg, MSG_MAX, "Startup\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  getInfo();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (0 != initMfrc522()) {
    // Retry init every 5 seconds
    HAL_Delay(5000);
  }

  int i = 0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    HAL_Delay(1000);

    snprintf(msg, MSG_MAX, "\r\nLoop: %d\r\n", ++i);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

//    getVersion();

//    getRandom();

//    readATQA();

//    readID();

    readNTAG();

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();

  /* basic deinit */
  mfrc522_basic_deinit();

  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
