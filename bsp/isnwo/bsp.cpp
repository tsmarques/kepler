#include "../trace.h"
#include <Config.hpp>

#include <driver/MPL3115A2.cpp>

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
}

void
bsp_init(mailbox_t* data_bus)
{
//  light_them_up();

  kepler::drivers::mpl3115a2::start(data_bus);
}