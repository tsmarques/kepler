#ifndef KEPLER_GPS_HPP
#define KEPLER_GPS_HPP

#include <Config.hpp>
#include <data/GpsFix.hpp>

static THD_FUNCTION(gps_driver_thread, arg)
{
  (void) arg;

  kepler::data::GpsFix fix;
  while (!chThdShouldTerminateX())
  {
    // dispatch gps fix data
    // (...)

    palToggleLine(LINE_LED3);
    chThdSleepMilliseconds(1000);
  }
}

thread_t*
startGpsDriver()
{
  return chThdCreateFromHeap(nullptr,
                             THD_WORKING_AREA_SIZE(128),
                             nullptr,
                             NORMALPRIO,
                             gps_driver_thread,
                             nullptr);
}

#endif
