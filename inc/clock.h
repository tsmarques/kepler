#ifndef KEPLER_CLOCK_H
#define KEPLER_CLOCK_H

#include <stdint.h>
#include <stdbool.h>

enum TIMER_ID
{
  //! Timer for main led blinking
  KEPLER_HEARTBEAT_TIMER = 0,
  //! Hack to know how many timers there are
  NUMBER_OF_TIMERS
};

//! Check if a given timer as overflown
bool
clk_overflow(uint8_t clk_id);

//! Update timers
void
clk_update();

//! Set timer value in milliseconds
void
clk_set_top(uint8_t id, float time_ms);

#endif
