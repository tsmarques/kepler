#ifndef KEPLER_IMU_HPP
#define KEPLER_IMU_HPP

#include <Config.hpp>
#include <data/Attitude.hpp>

static THD_FUNCTION(imu_driver_thread, arg)
{
  (void) arg;

  kepler::data::Attitude att;
  int counter = 0;
  while (!chThdShouldTerminateX())
  {
    att.m_phi = counter++;
    att.m_theta = counter++;
    att.m_psi = counter++;

    // dispatch attitude data
    // (...)

    palToggleLine(LINE_LED2);
    chThdSleepMilliseconds(1000 / IMU_DRIVER_FREQ);
  }
}

thread_t*
startImuDriver()
{
  return chThdCreateFromHeap(nullptr,
                             THD_WORKING_AREA_SIZE(128),
                             nullptr,
                             NORMALPRIO,
                             imu_driver_thread,
                             nullptr);
}

#endif
