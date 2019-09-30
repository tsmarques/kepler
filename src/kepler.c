#include <kepler.h>

//! HAL inclues
#include <stm32f7xx_hal.h>

//! Kepler
#include <usart.h>
#include <imu.h>
#include <icm20948.h>
//! Altimeter
#include <mpl315a2.h>

//! IMC includes
#include <imc-c/Acceleration.h>
#include <imc-c/EstimatedState.h>
#include <imc-c/LoggingControl.h>
#include <imc-c/Heartbeat.h>
#include <imc-c/GpsFix.h>
#include <imc-c/AngularVelocity.h>
#include <imc-c/Pressure.h>

static const char* hello_msg =
 "\x4e\x6f\x73\x79\x20\x62\x61\x73\x74\x61\x72\x64"
 "\x20\x61\x69\x6e\x27\x74\x20\x79\x61\x3f\x5c\x6e";

//! IMC accelerations' message
static Acceleration imc_accel;

//! IMC angular velocities' message
static AngularVelocity imc_angular_vel;

//! IMC pressure message
static Pressure imc_pressure;

//! ICM20938 imu device
static imu_t icm20948;


int
kepler_main(void)
{
  printf("%s\n", hello_msg);

  // register ICM20948 imu
  icm_register_device(&icm20948);
  icm20948.initialize();

  imc_accel = Acceleration_new();
  imc_angular_vel = AngularVelocity_new();
  imc_pressure = Pressure_new();

  int running = 1;
  mpl_init();
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
  while(running)
  {
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);

    HAL_Delay(500);

    icm20948.get_accelerations(&imc_accel.x, &imc_accel.y, &imc_accel.z);
    icm20948.get_angular_velocities(&imc_angular_vel.x, &imc_angular_vel.y, &imc_angular_vel.z);
    printf("ax %f ; ay %f ; az %f\r\n", imc_accel.x, imc_accel.y, imc_accel.z);

    if (mpl_is_on())
    {
      imc_pressure.value = mpl_read_pressure() * 0.01;
      printf("alt: %f\r\n", mpl_read_altitude());
      printf("press: %f\r\n", mpl_read_pressure());
      printf("temp: %f\r\n", mpl_read_temperature());
    }

    HAL_Delay(100);
  }

  return 0;
}
