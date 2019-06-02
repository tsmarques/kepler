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

static const char* hello_msg = "Nosy bastard ain't ya?\n";

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
    imu_get_accelerations(&imc_accel.x, &imc_accel.y, &imc_accel.z);
    imu_get_angular_velocities(&imc_angular_vel.x, &imc_angular_vel.y, &imc_angular_vel.z);

    // imc_accel.time = ...
    // imc_angular_vel = ...

    HAL_Delay(1000);
  }
}
