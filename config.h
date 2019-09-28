#ifndef __KEPLER_CONFIG_H__
#define __KEPLER_CONFIG_H__

#if defined(STM32F767xx)
#include "conf/stm32f767zi-discovery.h"


#else
#error "No valid configuration found"
#endif

#endif
