#include <clock.h>

#include <stm32f7xx_hal.h>

//! Avaiable timers' remaining time in milliseconds
static uint32_t clk_timers[NUMBER_OF_TIMERS] = {0};
//! Time in ms when a clock was set
static uint32_t clk_timers_start[NUMBER_OF_TIMERS] = {0};

bool
clk_overflow(uint8_t clk_id)
{
  assert_param(clk_id >= 0);
  return HAL_GetTick() - clk_timers_start[clk_id] >= (clk_timers[clk_id] + HAL_TICK_FREQ_DEFAULT);
}

void
clk_set_top(uint8_t id, float time_ms)
{
  assert_param(id >= 0);
  clk_timers[id] = time_ms;
  clk_timers_start[id] = HAL_GetTick();
}