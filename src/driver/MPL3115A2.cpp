#ifndef KEPLER_DRIVER_MPL3115A2_HPP
#define KEPLER_DRIVER_MPL3115A2_HPP

#include <Config.hpp>
#include <data/Pressure.hpp>
#include <data/Temperature.hpp>

#include "MPL3115A2.hpp"

namespace kepler::drivers::mpl3115a2
{
  static const I2CConfig I2C2_CONFIG = {
      .timingr = STM32_TIMINGR_PRESC(2U) | STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(0U)
                 | STM32_TIMINGR_SCLH(71U) | STM32_TIMINGR_SCLL(104U),
      .cr1 = 0,
      .cr2 = 0
  };

  //! Altitude data coming from OUT_P_MSB is scaled with this
  const unsigned c_altitude_scale_factor = 65536;
//! Pressure data coming from OUT_P_MSB is scaled with this
  const uint8_t c_pressure_scale_factor = 64;
//! Temperature data coming from OUT_T_MSB is scaled with this
  const uint16_t c_temperature_scale_factor =  256;

  static msg_t
  txRx(uint8_t* tx, size_t txsize, uint8_t* rxbfr, size_t rxbfr_size, unsigned wait = 500)
  {
    i2cAcquireBus(&I2CD2);
    msg_t rv = i2cMasterTransmitTimeout(&I2CD2, MPL_I2C_ADDR, tx, txsize, rxbfr, rxbfr_size, TIME_MS2I(wait));
    i2cReleaseBus(&I2CD2);

    return rv;
  }

  msg_t
  read(uint8_t reg, uint8_t* rxbfr, size_t rx_size)
  {
    i2cAcquireBus(&I2CD2);
    msg_t rv = i2cMasterReceiveTimeout(&I2CD2, reg, rxbfr, rx_size, TIME_MS2I(100));
    i2cReleaseBus(&I2CD2);

    return rv;
  }

  msg_t
  writeRegister(uint8_t addr, uint8_t value)
  {
    uint8_t data[] {addr, value};
    return txRx(data, sizeof(data), nullptr, 0);
  }

  msg_t
  readRegister(uint8_t addr, uint8_t& value, unsigned timeout = 500)
  {
    return txRx(&addr, 1, &value, 1, timeout);
  }

  bool
  poll(unsigned timeout)
  {
    uint8_t value = 0;
    if (readRegister(0x06, value, timeout) != MSG_OK)
      return false;
    return (value & 0x08) != 0;
  }

  msg_t
  whoAmI(uint8_t* dev_id)
  {
    uint8_t cmd_whoami = REG_WHO_AM_I;
    return txRx(&cmd_whoami, 1, dev_id, 1);
  }

  msg_t
  deactivate()
  {
    uint8_t ctrl_reg_1;
    msg_t ret = readRegister(CTRL_REG1, ctrl_reg_1);
    if (ret != MSG_OK)
      return ret;

    ctrl_reg_1 &= 0xfe;

    uint8_t tx_data[] {CTRL_REG1, ctrl_reg_1};
    return txRx(tx_data, sizeof(tx_data), nullptr, 0);
  }

  msg_t
  activate()
  {
    uint8_t cmd = CTRL_REG1;
    uint8_t ctrl_reg_1;

    msg_t ret = txRx(&cmd, 1, &ctrl_reg_1, 1);
    if (ret != MSG_OK)
      return ret;

    ctrl_reg_1 |= 0x01;

    uint8_t tx_data[] {CTRL_REG1, ctrl_reg_1};
    return txRx(tx_data, sizeof(tx_data), nullptr, 0);
  }

  inline constexpr void
  setOSRBit(uint8_t& ctrl_reg_1)
  {
#if (KEPLER_ALTIMETER_PERIOD >= 512)
    #warning "Oversampling maximum is 512ms"
    ctrl_reg_1 |= (OSR_128 << 3);
#elif (OSR_FREQ_BETWEEN(512, 258))
    ctrl_reg_1 |= (OSR_64 << 3);
#elif (OSR_FREQ_BETWEEN(258, 130))
    ctrl_reg_1 |= (OSR_32 << 3);
#elif (OSR_FREQ_BETWEEN(130, 66))
    ctrl_reg_1 |= (OSR_16 << 3);
#elif (OSR_FREQ_BETWEEN(66, 34))
      ctrl_reg_1 |= (OSR_8 << 3);
#elif (OSR_FREQ_BETWEEN(34, 18))
      ctrl_reg_1 |= (OSR_4 << 3);
#elif (OSR_FREQ_BETWEEN(18, 10))
      ctrl_reg_1 |= (OSR_2 << 3);
#elif (OSR_FREQ_BETWEEN(10, 6))
      ctrl_reg_1 |= (OSR_1 << 3);
#elif (KEPLER_ALTIMETER_PERIOD == 6)
    ctrl_reg_1 |= (OSR_1 << 3);
#else
#warning "Oversampling minimum is 6ms"
    ctrl_reg_1 |= (OSR_1 << 3);
#endif
  }

  inline void
  setAltimeterModeBit(uint8_t& ctrl_reg_1)
  {
    ctrl_reg_1 |= CTRL_REG1_BIT_ALT;
  }

  inline void
  setBarometerModeBit(uint8_t& ctrl_reg_1)
  {
    ctrl_reg_1 &= ~(CTRL_REG1_BIT_ALT);
  }

  msg_t
  setAllDataEvents()
  {
    uint8_t pt_data_cfg[] {PT_DATA_CFG, PT_DATA_CFG_BIT_DREM | PT_DATA_CFG_BIT_PDEFE | PT_DATA_CFG_BIT_TDEFE};
    return txRx(pt_data_cfg, sizeof(pt_data_cfg), nullptr, 0);
  }

  msg_t
  readData(float& value_p, float& value_t)
  {
    uint8_t cmd = OUT_P_MSB;
    uint8_t data[5];

    auto ret = txRx(&cmd, 1, data, sizeof(data));
    if (ret != MSG_OK)
      return ret;

    value_p = ((float)((data[0] << 16) | (data[1] << 8) | data[2]))
              / (float) c_pressure_scale_factor;

    value_t = (((uint16_t) (data[3] << 8)) | data[4]) /
              c_temperature_scale_factor;

    return MSG_OK;
  }

  static THD_FUNCTION(altimeter_driver_thread, arg)
  {
    mailbox_t* data_bus = (mailbox_t*)arg;

    trace("mpl3115a2: starting driver\r\n");
    i2cStart(&I2CD2, &I2C2_CONFIG);

    trace("mpl3115a2: requesting device ID\r\n");
    uint8_t dev_id;
    if(whoAmI(&dev_id) == MSG_OK && dev_id == DEVICE_ID)
      trace("mpl3115a2: 0x%02X\r\n", dev_id);
    else
      trace("mpl: failed to read id\r\n");

    if (deactivate() != MSG_OK)
    {
      trace("mpl: failed deactivation\r\n");
    }

    uint8_t ctrl_reg_1 = 0;
    // barometer mode
    setBarometerModeBit(ctrl_reg_1);
    // set oversampling according to driver's sampling frequency
    setOSRBit(ctrl_reg_1);

    trace("CTRL: %02x\r\n", ctrl_reg_1);
    if (writeRegister(CTRL_REG1, ctrl_reg_1) != MSG_OK)
    {
      trace("mpl: failed to set control register\r\n");
    }

    if (setAllDataEvents() != MSG_OK)
    {
      trace("mpl: failed to activate data events\r\n");
    }

    if (activate() != MSG_OK)
    {
      trace("mpl: failed activation\r\n");
    }

    kepler::data::Pressure p;
    kepler::data::Temperature t;
    while (!chThdShouldTerminateX())
    {
      if (readData(p.value, t.value) == MSG_OK)
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
  start(mailbox_t* data_bus)
  {
    return chThdCreateFromHeap(nullptr, THD_WORKING_AREA_SIZE(128), nullptr, NORMALPRIO, altimeter_driver_thread, data_bus);
  }
}
#endif
