#ifndef KEPLER_DRIVER_MPL3115A2_HPP
#define KEPLER_DRIVER_MPL3115A2_HPP

#include <Config.hpp>
#include <data/Pressure.hpp>
#include <data/Temperature.hpp>

#include "../../../bsp/trace.h"
#include "MPL3115A2.hpp"

using namespace kepler::drivers;

static MPL3115A2 drv;

static THD_FUNCTION(altimeter_driver_thread, arg)
{
  mailbox_t* data_bus = (mailbox_t*) arg;

  trace("mpl3115a2: requesting device ID\r\n");
  uint8_t dev_id;
  if(drv.whoAmI(dev_id) == MSG_OK && dev_id == MPL3115A2::c_device_id)
    trace("mpl3115a2: 0x%02X\r\n", dev_id);
  else
    trace("mpl: failed to read id\r\n");

  if (drv.deactivate() != MSG_OK)
  {
    trace("mpl: failed deactivation\r\n");
  }

  uint8_t ctrl_reg_1 = 0;
  // barometer mode
  drv.setBarometerModeBit(ctrl_reg_1);
  // set oversampling according to driver's sampling frequency
  drv.setOSRBit(ctrl_reg_1);

  trace("CTRL: %02x\r\n", ctrl_reg_1);
  if (drv.writeRegister(MPL3115A2::CTRL_REG_1, ctrl_reg_1) != MSG_OK)
  {
    trace("mpl: failed to set control register\r\n");
  }

  if (drv.setAllDataEvents() != MSG_OK)
  {
    trace("mpl: failed to activate data events\r\n");
  }

  if (drv.activate() != MSG_OK)
  {
    trace("mpl: failed activation\r\n");
  }

  kepler::data::Pressure p;
  kepler::data::Temperature t;
  while (!chThdShouldTerminateX())
  {
    if (drv.readData(p.value, t.value) == MSG_OK)
    {
      if (chMBPostTimeout(data_bus, (msg_t)&p, TIME_IMMEDIATE) != MSG_OK)
        trace("pressure: failed\r\n");

      if (chMBPostTimeout(data_bus, (msg_t)&t, TIME_IMMEDIATE) != MSG_OK)
        trace("temperature: failed\r\n");
    }

    chThdSleepMilliseconds(KEPLER_ALTIMETER_PERIOD);
  }
}

thread_t*
kepler::drivers::MPL3115A2::start(mailbox_t* bus, I2CDriver* plat_drv)
{
  drv.initialize(plat_drv);

  return chThdCreateFromHeap(nullptr,
                             THD_WORKING_AREA_SIZE(128),
                             nullptr,
                             NORMALPRIO,
                             altimeter_driver_thread,
                             bus);
}
#endif
