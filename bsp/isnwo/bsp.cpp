#include "../trace.h"
#include <Config.hpp>

void
bsp_init(mailbox_t* data_bus)
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