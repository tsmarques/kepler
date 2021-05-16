#ifndef KEPLER_IMU_HPP
#define KEPLER_IMU_HPP

#include <Config.hpp>

#if KEPLER_USE_IMU

#include <ch.h>
#include <board.h>
#include <data/Attitude.hpp>

static THD_FUNCTION(imu_driver_thread, arg)
{
  mailbox_t* data_bus = (mailbox_t*) arg;

  kepler::data::Attitude att;
  int counter = 0;
  while (!chThdShouldTerminateX())
  {
    att.m_phi = counter++;
    att.m_theta = counter++;
    att.m_psi = counter++;

    if (chMBPostTimeout(data_bus, (msg_t)&att, TIME_MS2I(100)) != MSG_OK)
        trace("imu: failed\r\n");

    palToggleLine(LINE_LED2);
    chThdSleepMilliseconds(1000 / IMU_DRIVER_FREQ);
  }
}

thread_t*
startImuDriver(mailbox_t* data_bus)
{
  return chThdCreateFromHeap(nullptr,
                             THD_WORKING_AREA_SIZE(128),
                             nullptr,
                             NORMALPRIO,
                             imu_driver_thread,
                             data_bus);
}

#endif

#endif
