#include <ch.h>
#include <hal.h>

#include "../bsp/trace.h"
#include <driver/IMU.hpp>
#include <driver/GPS.hpp>

int main()
{
  halInit();
  chSysInit();

  trace("starting imu driver\r\n");
  if (startImuDriver() == nullptr)
      trace("failed\r\n");

  trace("starting gps driver\r\n");
  if (startGpsDriver() == nullptr)
      trace("failed\r\n");

  while (!chThdShouldTerminateX())
  {
    palToggleLine(LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}
