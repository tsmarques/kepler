#include <kepler.hpp>

#include "../bsp/trace.h"
#include <data/Attitude.hpp>
#include <data/GpsFix.hpp>
#include <data/Pressure.hpp>
#include <data/Temperature.hpp>

extern void bsp_init(mailbox_t* data_bus);

int main()
{
  halInit();
  chSysInit();

  mailbox_t data_bus;
  msg_t data_msg[10];
  chMBObjectInit(&data_bus, data_msg, 10);

  bsp_init(&data_bus);
}
