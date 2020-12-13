//
// Created by tsm on 9/21/19.
//

#include "mpl315a2.h"
#include <stm32f7xx_hal.h>

#include <config.h>

extern I2C_HandleTypeDef MPL_I2C_HANDLER;


//! Altitude data coming from OUT_P_MSB is scaled with this
const unsigned c_altitude_scale_factor = 65536;
//! Pressure data coming from OUT_P_MSB is scaled with this
const uint8_t c_pressure_scale_factor = 64;
//! Temperature data coming from OUT_T_MSB is scaled with this
const uint16_t c_temperature_scale_factor =  256;

static inline uint8_t
read_bytes(uint16_t addr, uint8_t* bytes, uint16_t n_bytes)
{
  return HAL_I2C_Mem_Read(&MPL_I2C_HANDLER, (MPL_I2C_ADDR << 1U), addr, I2C_MEMADD_SIZE_8BIT, bytes, n_bytes, 100);
}

static inline uint8_t
write_bytes(uint16_t addr, uint8_t* bytes, uint16_t n_bytes)
{
  return HAL_I2C_Mem_Write(&MPL_I2C_HANDLER, (MPL_I2C_ADDR << 1U), addr, I2C_MEMADD_SIZE_8BIT, bytes, n_bytes, 100);
}

static inline void
activate()
{
  uint8_t ctrl_reg1;
  read_bytes(CTRL_REG1, &ctrl_reg1, 1);

  ctrl_reg1 |= CTRL_REG1_BIT_SBYB;
  write_bytes(CTRL_REG1, &ctrl_reg1, 1);
}

static inline void
deactivate()
{
  uint8_t ctrl_reg1;
  read_bytes(CTRL_REG1, &ctrl_reg1, 1);
  ctrl_reg1 &= ~(CTRL_REG1_BIT_SBYB);
  write_bytes(CTRL_REG1, &ctrl_reg1, 1);
}


static inline void
set_altimeter_mode()
{
  deactivate();

  uint8_t ctrl_reg1;
  read_bytes(CTRL_REG1, &ctrl_reg1, 1);
  // set altimeter
  ctrl_reg1 |= CTRL_REG1_BIT_ALT;
  // activate
  ctrl_reg1 |= CTRL_REG1_BIT_SBYB;
  write_bytes(CTRL_REG1, &ctrl_reg1, 1);
}

static inline void
set_barometer_mode()
{
  deactivate();

  uint8_t ctrl_reg1;
  read_bytes(CTRL_REG1, &ctrl_reg1, 1);
  // reset altimeter
  ctrl_reg1 &= ~(CTRL_REG1_BIT_ALT);
  // activate
  ctrl_reg1 |= CTRL_REG1_BIT_SBYB;
  write_bytes(CTRL_REG1, &ctrl_reg1, 1);
}

static inline void
set_oversample(uint8_t byte)
{
  uint8_t ctrl_reg1;
  read_bytes(CTRL_REG1, &ctrl_reg1, 1);

  ctrl_reg1 |= byte;
  write_bytes(CTRL_REG1, &ctrl_reg1, 1);
}

//! TODO Make it non-blocking
void
wait_data(uint8_t event_flag, uint16_t timeout_ms)
{
  uint8_t status;
  read_bytes(REG_STATUS, &status, 1);

  uint32_t start_t = HAL_GetTick();
  while ((status & event_flag) == 0 && (HAL_GetTick() - start_t < timeout_ms))
    read_bytes(REG_STATUS, &status, 1);
}

void
mpl_init()
{
  // standy mode
  deactivate();

  // enable data ready flags
  uint8_t byte = PT_DATA_CFG_BIT_DREM | PT_DATA_CFG_BIT_PDEFE | PT_DATA_CFG_BIT_TDEFE;
  write_bytes(PT_DATA_CFG, &byte, 1);

  // set 128 oversample ratio
  set_oversample(CTRL_REG1_BIT_OS0 | CTRL_REG1_BIT_OS1 | CTRL_REG1_BIT_OS2);

  // active mode
  activate();
}

float
mpl_read_pressure()
{
  set_barometer_mode();
  wait_data(REG_STATUS_BIT_PDR, 200);

  uint8_t pbyte[3] = {0x00};
  read_bytes(OUT_P_MSB, pbyte, 3);

  uint32_t msb = pbyte[0] << 16;
  return((float)(msb | (pbyte[1] << 8) | pbyte[2])) / (float) c_pressure_scale_factor;
}

float
mpl_read_altitude()
{
  set_altimeter_mode();
  wait_data(REG_STATUS_BIT_PDR, 200);

  uint8_t altitude_bytes[3] = {0x00};
  read_bytes(OUT_P_MSB, altitude_bytes, 3);

  uint32_t altitude = ((uint32_t) (altitude_bytes[0] << 24)) |
      (altitude_bytes[1] << 16) |
      (altitude_bytes[2] << 8);
  return (float)(altitude / (float) c_altitude_scale_factor);
}

float
mpl_read_temperature(void)
{
  wait_data(REG_STATUS_BIT_TDR, 200);

  uint8_t temperature[2] = {0x00};
  read_bytes(OUT_T_MSB, temperature,2);
  return (((uint16_t) (temperature[0] << 8)) | temperature[1]) / c_temperature_scale_factor;
}

bool
mpl_is_on()
{
  uint8_t byte = 0;
  return read_bytes(REG_WHO_AM_I, &byte, 1) == HAL_OK && byte == DEVICE_ID;
}
