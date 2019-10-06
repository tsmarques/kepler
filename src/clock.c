#include <config.h>
#include <clock.h>

#include <stm32f7xx_hal.h>

//! Avaiable timers' remaining time in milliseconds
static double clk_timers[NUMBER_OF_TIMERS] = {0};
//! Time in ms when a clock was set
static double clk_timers_start[NUMBER_OF_TIMERS] = {0};

bool
clk_overflow(uint8_t clk_id)
{
  assert_param(clk_id);
  return clk_timers[clk_id] == 0;
}

void
clk_update()
{
  // FIXME loop unroll?
  for(int i = 0; i < NUMBER_OF_TIMERS; ++i)
  {
    clk_timers[i] = clk_timers[i] - (HAL_GetTick() - clk_timers_start[i]);
    if (clk_timers[i] < 0)
      clk_timers[i] = 0;
  }
}

void
clk_set_top(uint8_t id, float time_ms)
{
  assert_param(id >= 0);
  clk_timers[id] = time_ms;
  clk_timers_start[id] = HAL_GetTick();
}