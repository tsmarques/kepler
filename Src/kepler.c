#include <kepler.h>

//! HAL inclues
#include <stm32f7xx_hal.h>

//! Kepler
#include <usart.h>
#include <imu.h>
#include <icm20948.h>

//! IMC includes
#include <imc-c/Acceleration.h>
#include <imc-c/EstimatedState.h>
#include <imc-c/LoggingControl.h>
#include <imc-c/Heartbeat.h>
#include <imc-c/GpsFix.h>
#include <imc-c/AngularVelocity.h>

static const char* hello_msg =
 "\x4e\x6f\x73\x79\x20\x62\x61\x73\x74\x61\x72\x64"
 "\x20\x61\x69\x6e\x27\x74\x20\x79\x61\x3f\x5c\x6e";

//! IMC accelerations' message
static Acceleration imc_accel;

//! IMC angular velocities' message
static AngularVelocity imc_angular_vel;

//! ICM20938 imu device
static imu_t icm20948;


int
kepler_main(void)
{
  printf("%s\n", hello_msg);

  // register ICM20948 imu
  icm_register_device(&icm20948);

  imc_accel = Acceleration_new();
  imc_angular_vel = AngularVelocity_new();

  int running = 1;
  while(running)
  {
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port,  LED_RED_Pin);
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
    HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);

    icm20948.get_accelerations(&imc_accel.x, &imc_accel.y, &imc_accel.z);
    icm20948.get_angular_velocities(&imc_angular_vel.x, &imc_angular_vel.y, &imc_angular_vel.z);

    // imc_accel.time = ...
    // imc_angular_vel = ...

    HAL_Delay(1000);
    printf("tick\n");
  }

  return 0;
}
