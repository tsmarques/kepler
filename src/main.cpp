#include <ch.h>
#include <hal.h>

#include "../bsp/trace.h"
#include <driver/IMU.hpp>
#include <driver/GPS.hpp>
#include <driver/Altimeter.hpp>

int main()
{
  halInit();
  chSysInit();

  mailbox_t data_bus;
  msg_t data_msg[10];
  chMBObjectInit(&data_bus, data_msg, 10);

#if KEPLER_USE_IMU
  trace("starting imu driver\r\n");
  if (startImuDriver(&data_bus) == nullptr)
      trace("failed\r\n");
#endif

#if KEPLER_USE_GPS
  trace("starting gps driver\r\n");
  if (startGpsDriver(&data_bus) == nullptr)
      trace("failed\r\n");
#endif

#if KEPLER_USE_ALTIMETER
  trace("starting altimeter driver\r\n");
  if (startAltimeterDriver(&data_bus) == nullptr)
    trace("failed\r\n");
#endif

  kepler::BasicData* msg;
  while (!chThdShouldTerminateX())
  {
    msg_t ret = chMBFetchTimeout(&data_bus, (msg_t *)&msg, chTimeMS2I(500));
    if (ret == MSG_OK)
    {
      switch (msg->getType())
      {
        case kepler::DT_ATTITUDE:
        {
          kepler::data::Attitude* att = static_cast<kepler::data::Attitude*>(msg);
          trace("Attitude | phi: %.3f theta: %.3f psi: %.3f\r\n",
                att->m_phi, att->m_theta, att->m_psi);
          break;
        }
        case kepler::DT_GPSFIX:
        {
          kepler::data::GpsFix* fix = static_cast<kepler::data::GpsFix*>(msg);
          trace("GpsFix | lat: %.3f lon: %.3f h: %.3f\r\n",
                fix->m_lat, fix->m_lon, fix->m_height);
          break;
        }
        case kepler::DT_PRESSURE:
        {
          kepler::data::Pressure* p = static_cast<kepler::data::Pressure*>(msg);
          trace("Pressure | value %.3f\r\n", p->value);
        }
      }
    }

    palToggleLine(LINE_LED1);
  }
}
