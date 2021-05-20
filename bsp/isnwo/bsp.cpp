#include "../trace.h"
#include <Config.hpp>

#include <data/Attitude.hpp>
#include <data/BasicData.hpp>
#include <data/GpsFix.hpp>
#include <data/Pressure.hpp>
#include <data/Temperature.hpp>
#include <driver/icm20948/ICM20948.hpp>
#include <driver/mpl3115a2/MPL3115A2.hpp>

void
light_them_up()
{
  palSetLine(LINE_LED_RED);

  chThdSleepMilliseconds(100);

  palClearLine(LINE_LED_RED);

  chThdSleepMilliseconds(100);

  palSetLine(LINE_LED_GREEN);

  chThdSleepMilliseconds(100);

  palClearLine(LINE_LED_GREEN);

  chThdSleepMilliseconds(100);

  palSetLine(LINE_LED_BLUE);

  chThdSleepMilliseconds(100);

  palClearLine(LINE_LED_BLUE);

  chThdSleepMilliseconds(100);

  palSetLine(LINE_LED_RED);

  chThdSleepMilliseconds(100);

  palClearLine(LINE_LED_RED);

  chThdSleepMilliseconds(100);

  palSetLine(LINE_LED_GREEN);

  chThdSleepMilliseconds(100);

  palClearLine(LINE_LED_GREEN);

  chThdSleepMilliseconds(100);

  palSetLine(LINE_LED_BLUE);

  chThdSleepMilliseconds(100);

  palClearLine(LINE_LED_BLUE);

  chThdSleepMilliseconds(100);

  palSetLine(LINE_LED_GREEN);

  palClearLine(LINE_LED_RED);
  palClearLine(LINE_LED_GREEN);
  palClearLine(LINE_LED_BLUE);
}

static const I2CConfig I2C2_CONFIG = {
    .timingr = STM32_TIMINGR_PRESC(2U) | STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(0U)
               | STM32_TIMINGR_SCLH(71U) | STM32_TIMINGR_SCLL(104U),
    .cr1 = 0,
    .cr2 = 0
};

void
bsp_init(mailbox_t* data_bus)
{
  light_them_up();

  trace("mpl3115a2: starting driver\r\n");
  i2cStart(&I2CD2, &I2C2_CONFIG);

  trace("mpl3115a2: starting driver\r\n");
//  kepler::drivers::MPL3115A2::start(data_bus, &I2CD2);
  kepler::drivers::ICM20948::start(data_bus, &SPID2);

  kepler::BasicData* msg;
  while (!chThdShouldTerminateX())
  {
    msg_t ret = chMBFetchTimeout(data_bus, (msg_t *)&msg, chTimeMS2I(500));
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
          break;
        }
        case kepler::DT_TEMPERATURE:
          kepler::data::Temperature* t = static_cast<kepler::data::Temperature*>(msg);
          trace("Temperature | value %.3f\r\n", t->value);
          break;
      }
    }
  }
}