#ifndef KEPLER_GPS_HPP
#define KEPLER_GPS_HPP

#include <Config.hpp>

#if KEPLER_USE_GPS

#include <data/GpsFix.hpp>

static THD_FUNCTION(gps_driver_thread, arg)
{
  mailbox_t* data_bus = (mailbox_t*) arg;

  kepler::data::GpsFix fix;
  fix.m_lat = 0.12;
  fix.m_lon = 0.02;
  fix.m_height = 11;

  while (!chThdShouldTerminateX())
  {
    if (chMBPostTimeout(data_bus, (msg_t)&fix, TIME_MS2I(100)) != MSG_OK)
      trace("gps: failed\r\n");

    palToggleLine(LINE_LED3);
    chThdSleepMilliseconds(1000);
  }
}

thread_t*
startGpsDriver(mailbox_t* data_bus)
{
  return chThdCreateFromHeap(nullptr,
                             THD_WORKING_AREA_SIZE(128),
                             nullptr,
                             NORMALPRIO,
                             gps_driver_thread,
                             data_bus);
}

#endif
#endif
