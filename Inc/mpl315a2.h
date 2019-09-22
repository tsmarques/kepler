//
// Created by tsm on 9/21/19.
//

#ifndef __KEPLER_MPL315A2_H
#define __KEPLER_MPL315A2_H

#include <stdbool.h>
#include <stdint.h>

//! Sensor Status Register
#define REG_STATUS   0x00
//! Device Identification Register
#define REG_WHO_AM_I 0x0c


//! Value in the who am i register
#define DEVICE_ID   0xC4

//! Returns if device is on and
//! we're communicating correctly
//! with it
bool
mpl_is_on();

#endif
