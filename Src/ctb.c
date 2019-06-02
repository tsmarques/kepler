#include "ctb.h"

//! HAL inclues
#include "stm32f7xx_hal.h"

//! CTB
#include <usart.h>
#include <icm20948.h>

//! IMC includes
#include <imc-c/Acceleration.h>
#include <imc-c/EstimatedState.h>
#include <imc-c/LoggingControl.h>
#include <imc-c/Heartbeat.h>
#include <imc-c/GpsFix.h>
#include <imc-c/AngularVelocity.h>

static const char* hello_msg = "\x4e\x6f\x73\x79\x20\x62\x61\x73\x74\x61\x72\x64\x20\x61\x69\x6e\x27\x74\x20\x79\x61\x3f\x5c\x6e";

static Acceleration imc_accel;
static AngularVelocity imc_angular_vel;

int
ctb_main(void)
{
  uart_print(hello_msg, strlen(hello_msg));

  imc_accel = Acceleration_new();
  imc_angular_vel = AngularVelocity_new();

  while(1)
  {
    icm_get_accelerations(&imc_accel.x, &imc_accel.y, &imc_accel.z);
    icm_get_angular_velocities(&imc_angular_vel.x, &imc_angular_vel.y, &imc_angular_vel.z);

    // imc_accel.time = ...
    // imc_angular_vel = ...

    HAL_Delay(1000);
  }
}
