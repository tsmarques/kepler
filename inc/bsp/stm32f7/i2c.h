#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern I2C_HandleTypeDef MPL_I2C_HANDLER;

void
MX_I2C2_Init(void);

#ifdef __cplusplus
}
#endif
#endif