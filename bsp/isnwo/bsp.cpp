#include "../trace.h"
#include <Config.hpp>

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

  kepler::drivers::MPL3115A2::start(data_bus, &I2CD2);
}