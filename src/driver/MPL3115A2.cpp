#ifndef KEPLER_DRIVER_MPL3115A2_HPP
#define KEPLER_DRIVER_MPL3115A2_HPP

#include <Config.hpp>
#include <data/Pressure.hpp>

#include "MPL3115A2.hpp"

namespace kepler::drivers::mpl3115a2
{

  static const I2CConfig I2C2_CONFIG = {
      .timingr = STM32_TIMINGR_PRESC(2U) | STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(0U)
                 | STM32_TIMINGR_SCLH(71U) | STM32_TIMINGR_SCLL(104U),
      .cr1 = 0,
      .cr2 = 0
  };

  static msg_t
  txRx(uint8_t* tx, size_t txsize, uint8_t* rxbfr, size_t rxbfr_size)
  {
    i2cAcquireBus(&I2CD2);
    msg_t rv = i2cMasterTransmitTimeout(&I2CD2, MPL_I2C_ADDR, tx, txsize, rxbfr, rxbfr_size, TIME_MS2I(500));
    i2cReleaseBus(&I2CD2);

    return rv;
  }

  msg_t
  read(uint8_t reg, uint8_t* rxbfr, size_t rx_size)
  {
    i2cAcquireBus(&I2CD2);
    msg_t rv = i2cMasterReceiveTimeout(&I2CD2, reg, rxbfr, rx_size, TIME_MS2I(500));
    i2cReleaseBus(&I2CD2);

    return rv;
  }

  msg_t whoAmI(uint8_t* dev_id)
  {
    uint8_t cmd_whoami = REG_WHO_AM_I;
    return txRx(&cmd_whoami, 1, dev_id, 1);
  }

  static THD_FUNCTION(altimeter_driver_thread, arg)
  {
    mailbox_t* data_bus = (mailbox_t*)arg;

    palSetLine(LINE_LED_BLUE);
    i2cStart(&I2CD2, &I2C2_CONFIG);

    palClearLine(LINE_LED_BLUE);
    palClearLine(LINE_LED_GREEN);
    palClearLine(LINE_LED_RED);

    palSetLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(500);
    palClearLine(LINE_LED_BLUE);
    uint8_t dev_id;

    if(whoAmI(&dev_id) == MSG_OK && dev_id == DEVICE_ID)
      palSetLine(LINE_LED_GREEN);
    else
      palSetLine(LINE_LED_RED);

    kepler::data::Pressure p;
    while (!chThdShouldTerminateX())
    {
      chThdSleepMilliseconds(500);
    }
  }

  thread_t*
  start(mailbox_t* data_bus)
  {
    return chThdCreateFromHeap(nullptr, THD_WORKING_AREA_SIZE(128), nullptr, NORMALPRIO, altimeter_driver_thread, data_bus);
  }
}
#endif
