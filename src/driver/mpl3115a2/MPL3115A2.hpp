#ifndef __KEPLER_MPL315A2_H
#define __KEPLER_MPL315A2_H

#include <stdbool.h>
#include <stdint.h>

#include <Config.hpp>
#include "Definitions.hpp"

namespace kepler::drivers
{
  class MPL3115A2
  {
    //! Altitude data coming from OUT_P_MSB is scaled with this
    const unsigned c_altitude_scale_factor = 65536;
    //! Pressure data coming from OUT_P_MSB is scaled with this
    const uint8_t c_pressure_scale_factor = 64;
    //! Temperature data coming from OUT_T_MSB is scaled with this
    const uint16_t c_temperature_scale_factor =  256;
    //! Platform's IO device
    I2CDriver* plat_io;

    msg_t
    txRx(uint8_t* tx, size_t txsize, uint8_t* rxbfr, size_t rxbfr_size, unsigned wait = 500) const
    {
      chDbgAssert(plat_io == nullptr, "no platform driver");

      i2cAcquireBus(plat_io);
      msg_t rv = i2cMasterTransmitTimeout(plat_io, MPL_I2C_ADDR, tx, txsize, rxbfr, rxbfr_size, TIME_MS2I(wait));
      i2cReleaseBus(plat_io);

      return rv;
    }

  public:
    static thread_t*
    start(mailbox_t* bus, I2CDriver* plat_drv);

    MPL3115A2() :
        plat_io(nullptr)
    { }

    void
    initialize(I2CDriver* plat_drv)
    {
      plat_io = plat_drv;
    }

    msg_t
    writeRegister(uint8_t addr, uint8_t value) const
    {
      uint8_t data[] {addr, value};
      return txRx(data, sizeof(data), nullptr, 0);
    }

    msg_t
    readRegister(uint8_t addr, uint8_t& value, unsigned timeout = 500) const
    {
      return txRx(&addr, 1, &value, 1, timeout);
    }

    bool
    poll(unsigned timeout) const
    {
      uint8_t value = 0;
      if (readRegister(0x06, value, timeout) != MSG_OK)
        return false;
      return (value & 0x08) != 0;
    }

    msg_t
    whoAmI(uint8_t* dev_id) const
    {
      uint8_t cmd_whoami = REG_WHO_AM_I;
      return txRx(&cmd_whoami, 1, dev_id, 1);
    }

    msg_t
    deactivate() const
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
    activate() const
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
    setOSRBit(uint8_t& ctrl_reg_1) const
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
    setAltimeterModeBit(uint8_t& ctrl_reg_1) const
    {
      ctrl_reg_1 |= CTRL_REG1_BIT_ALT;
    }

    inline void
    setBarometerModeBit(uint8_t& ctrl_reg_1) const
    {
      ctrl_reg_1 &= ~(CTRL_REG1_BIT_ALT);
    }

    msg_t
    setAllDataEvents() const
    {
      uint8_t pt_data_cfg[] {PT_DATA_CFG, PT_DATA_CFG_BIT_DREM | PT_DATA_CFG_BIT_PDEFE | PT_DATA_CFG_BIT_TDEFE};
      return txRx(pt_data_cfg, sizeof(pt_data_cfg), nullptr, 0);
    }

    msg_t
    readData(float& value_p, float& value_t) const
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
  };
}

#endif
