#ifndef __KEPLER_MPL315A2_H
#define __KEPLER_MPL315A2_H

#include <stdbool.h>
#include <stdint.h>

namespace kepler::drivers::mpl3115a2
{
//! Sensor Status Register
#define REG_STATUS 0x00
//! Data ready register
#define REG_DR_STATUS 0x06

//! Pressure/Altitude/Temperature data ready
#define REG_STATUS_BIT_PTDR (1 << 3U)
//! Pressure/Altitude new data is available
#define REG_STATUS_BIT_PDR (1 << 2U)
//! Temperature new Data Available.
#define REG_STATUS_BIT_TDR (1 << 1U)

#define OSR_FREQ_BETWEEN(x, y) (KEPLER_ALTIMETER_PERIOD) < x && KEPLER_ALTIMETER_PERIOD >= y

//! Device Identification Register
#define REG_WHO_AM_I 0x0c

//! Control register 1
#define CTRL_REG1 0x26
#define CTRL_REG1_BIT_ALT 0x80
#define CTRL_REG1_BIT_RAW 0x40
#define CTRL_REG1_BIT_OS2 0x20
#define CTRL_REG1_BIT_OS1 0x10
#define CTRL_REG1_BIT_OS0 0x08
#define CTRL_REG1_BIT_RST 0x04
#define CTRL_REG1_BIT_OST 0x02
//! Active/Standby bit from CTRL_REG1
#define CTRL_REG1_BIT_SBYB 0x01

#define OSR_1              0x0
#define OSR_2              0x1
#define OSR_4              0x2
#define OSR_8              0x3
#define OSR_16             0x4
#define OSR_32             0x5
#define OSR_64             0x6
#define OSR_128            0x7

//! Pressure/Altitude Data Out MSB
#define OUT_P_MSB 0x01
//! Temperature Data OUT MSB
#define OUT_T_MSB 0x04

//! Value in the who am i register
#define DEVICE_ID 0xC4

//! Data ready flags
#define PT_DATA_CFG 0x13
//! Data ready event mode bit
#define PT_DATA_CFG_BIT_DREM 0x04
//! Altitude/Pressure data event flag
#define PT_DATA_CFG_BIT_PDEFE 0x02
//! Temperature data event flag
#define PT_DATA_CFG_BIT_TDEFE 0x01

  thread_t*
  start(mailbox_t* data_bus);
}

#endif
