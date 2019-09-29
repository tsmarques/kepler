#ifndef __KEPLER_CONFIG_H__
#define __KEPLER_CONFIG_H__

#if defined(STM32F767xx)
#include "conf/stm32f767zi-discovery.h"

#elif defined(STM32F767xx_TESTBOARD_1)
#include "conf/stm32f767-testboard-1.h"

#else
#error "No valid configuration found"
#endif

#endif
