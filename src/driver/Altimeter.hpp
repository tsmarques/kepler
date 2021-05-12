#ifndef KEPLER_ALTIMETER_HPP
#define KEPLER_ALTIMETER_HPP

#include <Config.hpp>

#if KEPLER_USE_ALTIMETER

#include <data/Pressure.hpp>

static THD_FUNCTION(altimeter_driver_thread, arg)
{
  mailbox_t* data_bus = (mailbox_t*) arg;

  kepler::data::Pressure p;
  while (!chThdShouldTerminateX())
  {
    p.value = 0;
    if (chMBPostTimeout(data_bus, (msg_t)&p, TIME_MS2I(100)) != MSG_OK)
      trace("alt: failed\r\n");

    chThdSleepMilliseconds(1000 / ALTIMETER_DRIVER_FREQ);
  }
}

thread_t*
startAltimeterDriver(mailbox_t* data_bus)
{
  return chThdCreateFromHeap(nullptr,
                             THD_WORKING_AREA_SIZE(128),
                             nullptr,
                             NORMALPRIO,
                             altimeter_driver_thread,
                             data_bus);
}

#endif
#endif
