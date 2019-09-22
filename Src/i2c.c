#include <i2c.h>
#include <config.h>

I2C_HandleTypeDef hi2c4;

void
MX_I2C4_Init(void)
{
  hi2c4.Instance = MPL_I2C_DEVICE;
  hi2c4.Init.Timing = 0x00303D5B;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

void
HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (i2cHandle->Instance == MPL_I2C_DEVICE)
  {
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**I2C4 GPIO Configuration
    PD13     ------> I2C4_SDA
    PD12     ------> I2C4_SCL
    */
    GPIO_InitStruct.Pin = MPL_SDA | MPL_SCL;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
    HAL_GPIO_Init(MPL_I2C_GPIO, &GPIO_InitStruct);

    __HAL_RCC_I2C4_CLK_ENABLE();
  }
}

void
HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if (i2cHandle->Instance==MPL_I2C_DEVICE)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C4_CLK_DISABLE();
  
    /**I2C4 GPIO Configuration
    PD13     ------> I2C4_SDA
    PD12     ------> I2C4_SCL
    */
    HAL_GPIO_DeInit(MPL_I2C_GPIO, MPL_SCL | MPL_SDA);
  }
}
