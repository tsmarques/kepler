/**
  ******************************************************************************
  * File Name          : SPI.c
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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

#include "spi.h"

#include <config.h>

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance == IMU_SPI_DEVICE)
  {
    IMU_RCC_ENABLE();
    IMU_GPIO_CLOCK_ENABLE();

    GPIO_InitStruct.Pin = IMU_CLK_Pin | IMU_MISO_Pin | IMU_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(IMU_PORT, &GPIO_InitStruct);
    HAL_Delay(2);

    GPIO_InitStruct.Pin = IMU_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(IMU_CS_PORT, &GPIO_InitStruct);
    HAL_Delay(2);

//    HAL_NVIC_SetPriority(IMU_IRQN, 0, 0);
//    HAL_NVIC_EnableIRQ(IMU_IRQN);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance == IMU_SPI_DEVICE)
  {
    IMU_RCC_DISABLE();
    HAL_GPIO_DeInit(IMU_PORT, IMU_CS_Pin | IMU_CLK_Pin | IMU_MISO_Pin | IMU_MOSI_Pin);
    HAL_NVIC_DisableIRQ(IMU_IRQN);
  }
}
