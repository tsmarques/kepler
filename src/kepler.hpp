#ifndef KEPLER_KEPLER_HPP
#define KEPLER_KEPLER_HPP

#include <ch.h>
#include <hal.h>

//! Convert Hz to period in milliseconds
#define KEPLER_Hz2MS(x) (1000 / x)

#define KEPLER_ALTIMETER_PERIOD     KEPLER_Hz2MS(5)
#define KEPLER_IMU_PERIOD           KEPLER_Hz2MS(20)

#endif
