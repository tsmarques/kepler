#include <kepler.h>

//! HAL inclues
#include <stm32f7xx_hal.h>

//! Kepler
#include <usart.h>
#include <imu.h>
#include <icm20948.h>
#include <debug.h>
#include <log.h>
#include <led.h>

//! Altimeter
#include <mpl315a2.h>
#include <clock.h>

//! IMC includes
#include <imc-c/Acceleration.h>
#include <imc-c/EstimatedState.h>
#include <imc-c/LoggingControl.h>
#include <imc-c/Heartbeat.h>
#include <imc-c/GpsFix.h>
#include <imc-c/AngularVelocity.h>
#include <imc-c/Pressure.h>

//! working buffer
static uint8_t bfr_work[1024];

static  unsigned serialization_size = 0;

static const char* hello_msg =
 "\x4e\x6f\x73\x79\x20\x62\x61\x73\x74\x61\x72\x64"
 "\x20\x61\x69\x6e\x27\x74\x20\x79\x61\x3f\x5c\x6e";

//! IMC accelerations' message
static Acceleration imc_accel;

//! IMC angular velocities' message
static AngularVelocity imc_angular_vel;

//! IMC pressure message
static Pressure imc_pressure;

//! IMC Heartbeat
static Heartbeat imc_hbeat;

//! ICM20938 imu device
static imu_t icm20948;

//! Utility macro to log an IMC message
#define log_imc(MSG_NAME, imc_msg) {\
    serialization_size = MSG_NAME##_serialization_size(&imc_msg); \
    imc_msg.m_header.timestamp = HAL_GetTick(); \
    MSG_NAME##_serialize(&imc_msg, bfr_work); \
    log_write(bfr_work, serialization_size); \
    bfr_work[0] = 0;\
}


int
kepler_main(void)
{
  trace("%s\n", hello_msg);

  // TODO handle error case
  if (!log_init())
    trace ("failed to mount disk\r\n");

  if (!log_open())
    trace("failed to open log\r\n");

  // register ICM20948 imu
  icm_register_device(&icm20948);
  icm20948.initialize();

  imc_hbeat = Heartbeat_new();
  imc_accel = Acceleration_new();
  imc_angular_vel = AngularVelocity_new();
  imc_pressure = Pressure_new();

  int running = 1;
  mpl_init();
  led_off(BLUE);

  // set timers
  clk_set_top(KEPLER_HEARTBEAT_TIMER, 1000);
  clk_set_top(KEPLER_NAV_DATA_TIMER, 1000 / 6.0);
  clk_set_top(KEPLER_HEARTBEAT_TIMER, 1000);
  clk_set_top(KEPLER_PRESSURE_DATA_TIMER, 500);

  while(running)
  {
    clk_update();
    log_update();

    if (clk_overflow(KEPLER_HEARTBEAT_TIMER))
    {
      led_toggle(GREEN);
      clk_set_top(KEPLER_HEARTBEAT_TIMER, 1000);

      log_imc(Heartbeat, imc_hbeat);
    }

    if (mpl_is_on() && clk_overflow(KEPLER_PRESSURE_DATA_TIMER))
    {
      imc_pressure.value = mpl_read_pressure() * 0.01;
      // log pressure
      log_imc(Pressure, imc_pressure);

      trace("alt: %f ; press: %f ; temp: %f\r\n",
             mpl_read_altitude(),
             imc_pressure.value,
             mpl_read_temperature());

      clk_set_top(KEPLER_PRESSURE_DATA_TIMER, 500);
    }

    if (clk_overflow(KEPLER_NAV_DATA_TIMER))
    {
      icm20948.get_accelerations(&imc_accel.x, &imc_accel.y, &imc_accel.z);
      icm20948.get_angular_velocities(&imc_angular_vel.x, &imc_angular_vel.y, &imc_angular_vel.z);
      trace("ax %f ; ay %f ; az %f\r\n", imc_accel.x, imc_accel.y, imc_accel.z);

      // log accelerations
      log_imc(Acceleration, imc_accel);
      // log angular velocities
      log_imc(AngularVelocity, imc_angular_vel);

      clk_set_top(KEPLER_NAV_DATA_TIMER, 1000 / 6.0);
    }
  }

  return 0;
}
