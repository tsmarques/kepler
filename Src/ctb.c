#include "ctb.h"

//! HAL inclues
#include "stm32f7xx_hal.h"

//! CTB
#include <uart.h>

//! IMC includes
#include <imc-c/Acceleration.h>
#include <imc-c/EstimatedState.h>
#include <imc-c/LoggingControl.h>
#include <imc-c/Heartbeat.h>
#include <imc-c/GpsFix.h>

int
ctb_main(void)
{
  const char* hello_msg = "Nosy bastard ain't ya?\n";
  while(1)
  {
    uart_print(hello_msg, strlen(hello_msg));
    HAL_Delay(1000);
  }
}
