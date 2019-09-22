/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_RED_Pin GPIO_PIN_1
#define LED_RED_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_2
#define LED_GREEN_GPIO_Port GPIOA
#define LED_BLUE_Pin GPIO_PIN_3
#define LED_BLUE_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_7
#define GPS_RX_GPIO_Port GPIOE
#define GPS_TX_Pin GPIO_PIN_8
#define GPS_TX_GPIO_Port GPIOE
#define IMU_SCK_Pin GPIO_PIN_13
#define IMU_SCK_GPIO_Port GPIOB
#define IMU_MISO_Pin GPIO_PIN_14
#define IMU_MISO_GPIO_Port GPIOB
#define IMU_MOSI_Pin GPIO_PIN_15
#define IMU_MOSI_GPIO_Port GPIOB
#define SDCARD_D0_Pin GPIO_PIN_8
#define SDCARD_D0_GPIO_Port GPIOC
#define SDCARD_D1_Pin GPIO_PIN_9
#define SDCARD_D1_GPIO_Port GPIOC
#define DEBUG_TX_Pin GPIO_PIN_12
#define DEBUG_TX_GPIO_Port GPIOA
#define SDCARD_D2_Pin GPIO_PIN_10
#define SDCARD_D2_GPIO_Port GPIOC
#define SDCARD_D3_Pin GPIO_PIN_11
#define SDCARD_D3_GPIO_Port GPIOC
#define SDCARD_CK_Pin GPIO_PIN_12
#define SDCARD_CK_GPIO_Port GPIOC
#define SDCARD_CMD_Pin GPIO_PIN_2
#define SDCARD_CMD_GPIO_Port GPIOD
#define GSM_RX_Pin GPIO_PIN_5
#define GSM_RX_GPIO_Port GPIOB
#define GSM_TX_Pin GPIO_PIN_6
#define GSM_TX_GPIO_Port GPIOB
#define TELEMETRY_RX_Pin GPIO_PIN_0
#define TELEMETRY_RX_GPIO_Port GPIOE
#define TELEMETRY_TX_Pin GPIO_PIN_1
#define TELEMETRY_TX_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
