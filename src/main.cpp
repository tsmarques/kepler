#include <ch.h>
#include <hal.h>

#include "../bsp/trace.h"
#include <driver/IMU.hpp>

int main()
{
    halInit();
    chSysInit();

    trace("starting imu driver\r\n");
    if (startImuDriver() == nullptr)
      trace("failed\r\n");

    while (!chThdShouldTerminateX())
    {
        palToggleLine(LINE_LED1);
        chThdSleepMilliseconds(500);
    }
}
