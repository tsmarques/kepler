#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern I2C_HandleTypeDef hi2c4;

void
MX_I2C4_Init(void);

#ifdef __cplusplus
}
#endif
#endif