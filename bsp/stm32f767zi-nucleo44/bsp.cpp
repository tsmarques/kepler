#include "../trace.h"
#include <Config.hpp>
#include <chmboxes.h>

#include <driver/Altimeter.hpp>
#include <driver/GPS.hpp>
#include <driver/IMU.hpp>

void
bsp_init(mailbox_t* data_bus)
{
#if KEPLER_USE_IMU
  trace("starting imu driver\r\n");
  if (startImuDriver(data_bus) == nullptr)
      trace("failed\r\n");
#endif

#if KEPLER_USE_GPS
  trace("starting gps driver\r\n");
  if (startGpsDriver(data_bus) == nullptr)
      trace("failed\r\n");
#endif

#if KEPLER_USE_ALTIMETER
  trace("starting altimeter driver\r\n");
  if (startAltimeterDriver(data_bus) == nullptr)
    trace("failed\r\n");
#endif
}