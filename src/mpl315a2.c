//
// Created by tsm on 9/21/19.
//

#include "mpl315a2.h"
#include <stm32f7xx_hal.h>

#include <config.h>

extern I2C_HandleTypeDef hi2c4;

static inline uint8_t
read_byte(uint16_t addr, uint8_t* value)
{
  return HAL_I2C_Mem_Read(&hi2c4, (MPL_I2C_ADDR << 1U), addr, I2C_MEMADD_SIZE_8BIT, value, 1, 100);
}

bool
mpl_is_on()
{
  uint8_t byte = 0;
  return read_byte(REG_WHO_AM_I, &byte) == HAL_OK && byte == DEVICE_ID;
}
