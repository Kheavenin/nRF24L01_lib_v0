/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "controleModule.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEST_0 1
#define TEST_1 1
#define TEST_2 0
#define TEST_3 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t thr1 = 1000;
uint32_t thr2 = 2000;
uint32_t thr3 = 3000;
uint32_t thr4 = 3000;

uint32_t testCounter = 0;
uint8_t statRegTmp = 0;
uint8_t confRegTmp = 0;
uint8_t obrvRegTmp = 0;
uint8_t retrRegTmp = 0;

uint16_t regAddrMax = 0x1D;
uint8_t regTmp = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */
	// HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_RESET);
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
#if TEST_0
	writeRegister(CONFIG, 0x02);
	regTmp = readRegister(CONFIG);
#endif

	while (1) {
#if TEST_3
		uint16_t i = 0;
		for (i = 0; i <= regAddrMax; i++) {
			regTmp = readRegister(i);
			//HAL_Delay(10);
		}
#endif

#if TEST_1
		testCounter++;
		if (testCounter == thr1) {
			uint8_t i;
			for (i = 0; i < 0x07; ++i) {
				writeRegister(i, 0x07);
			}
		}
		if (testCounter == thr2) {
			uint8_t i;
			for (i = 0; i < 0x07; ++i) {
				regTmp = readRegister(i);
			}
		}
		if (testCounter == thr3) {
			uint8_t i;
			for (i = 0; i < 0x07; ++i) {
				writeRegister(CONFIG, 0x07);
			}
			regTmp = readRegister(CONFIG);
		}
		if (testCounter == thr4) {
			testCounter = 0;
		}
#endif

#if TEST_2
		testCounter++;
		if (testCounter == thr1) {
			confRegTmp = 0;
			obrvRegTmp = 0;
			retrRegTmp = 0;
			statRegTmp = 0;

			writeRegister(CONFIG, 0x07);
			confRegTmp = readRegister(CONFIG);

			writeRegister(OBSERVE_TX, 0x1F);
			obrvRegTmp = readRegister(OBSERVE_TX);

			writeRegister(SETUP_RETR, 0xA3);
			retrRegTmp = readRegister(SETUP_RETR);

			/*	confRegTmp = readRegister(CONFIG);
			 obrvRegTmp = readRegister(OBSERVE_TX);
			 retrRegTmp = readRegister(SETUP_RETR);*/

			statRegTmp = getStatus();
		}
		if (testCounter == thr2) {
			confRegTmp = 0;
			obrvRegTmp = 0;
			retrRegTmp = 0;
			statRegTmp = 0;

			writeRegister(CONFIG, 0x02);
			confRegTmp = readRegister(CONFIG);

			writeRegister(OBSERVE_TX, 0xF1);
			obrvRegTmp = readRegister(OBSERVE_TX);

			writeRegister(SETUP_RETR, 0x2B);
			retrRegTmp = readRegister(SETUP_RETR);

			/*	confRegTmp = readRegister(CONFIG);
			 obrvRegTmp = readRegister(OBSERVE_TX);
			 retrRegTmp = readRegister(SETUP_RETR);*/
			statRegTmp = getStatus();
		}
		if (testCounter == thr3) {
			confRegTmp = 0;
			obrvRegTmp = 0;
			retrRegTmp = 0;
			statRegTmp = 0;

			writeRegister(CONFIG, 0x07);
			retrRegTmp = readRegister(SETUP_RETR);

			writeRegister(OBSERVE_TX, 0xA2);
			confRegTmp = readRegister(CONFIG);

			writeRegister(SETUP_RETR, 0x3C);
			obrvRegTmp = readRegister(OBSERVE_TX);
			writeRegister(CONFIG, 0x02);
			retrRegTmp = readRegister(SETUP_RETR);

		}
		if (testCounter == thr4) {
			testCounter = 0;
		}
#endif

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
	/* End of main() */
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
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

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
