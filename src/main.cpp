#include <kepler.hpp>

#include "../bsp/trace.h"
#include <data/Attitude.hpp>
#include <data/GpsFix.hpp>
#include <data/Pressure.hpp>

extern void bsp_init(mailbox_t* data_bus);

int main()
{
  halInit();
  chSysInit();

  mailbox_t data_bus;
  msg_t data_msg[10];
  chMBObjectInit(&data_bus, data_msg, 10);

  bsp_init(&data_bus);

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
  }
}
