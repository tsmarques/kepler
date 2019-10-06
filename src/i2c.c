#include <i2c.h>
#include <config.h>

I2C_HandleTypeDef MPL_I2C_HANDLER;

void
MX_I2C2_Init(void)
{
  MPL_I2C_HANDLER.Instance = MPL_I2C_DEVICE;
  MPL_I2C_HANDLER.Init.Timing = 0x00303D5B;
  MPL_I2C_HANDLER.Init.OwnAddress1 = 0;
  MPL_I2C_HANDLER.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  MPL_I2C_HANDLER.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  MPL_I2C_HANDLER.Init.OwnAddress2 = 0;
  MPL_I2C_HANDLER.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  MPL_I2C_HANDLER.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  MPL_I2C_HANDLER.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&MPL_I2C_HANDLER) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&MPL_I2C_HANDLER, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigDigitalFilter(&MPL_I2C_HANDLER, 0) != HAL_OK)
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
    MPL_RCC_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = MPL_SDA | MPL_SCL;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(MPL_I2C_GPIO, &GPIO_InitStruct);

    MPL_RCC_I2C_CLK_ENABLE();
  }
}

void
HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if (i2cHandle->Instance==MPL_I2C_DEVICE)
  {
    /* Peripheral clock disable */
    MPL_RCC_I2C_CLK_DISABLE();

    HAL_GPIO_DeInit(MPL_I2C_GPIO, MPL_SCL | MPL_SDA);
  }
}
