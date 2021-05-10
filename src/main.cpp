#include <ch.h>
#include <hal.h>

#include "../bsp/trace.h"

int main()
{
    halInit();
    chSysInit();

    trace("init\r\n");
    while (!chThdShouldTerminateX())
    {
        palSetLine(LINE_LED1);
        chThdSleepMilliseconds(50);
        palSetLine(LINE_LED2);
        chThdSleepMilliseconds(50);
        palSetLine(LINE_LED3);
        chThdSleepMilliseconds(200);
        palClearLine(LINE_LED1);
        chThdSleepMilliseconds(50);
        palClearLine(LINE_LED2);
        chThdSleepMilliseconds(50);
        palClearLine(LINE_LED3);
        chThdSleepMilliseconds(200);
    }
}
