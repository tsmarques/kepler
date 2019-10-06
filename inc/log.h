#ifndef KEPLER_LOG_H
#define KEPLER_LOG_H

#include <stdbool.h>
#include <stdint.h>

void MX_DMA_Init(void);

//! Current log size
uint16_t
log_size();

//! Initialize peripheral
bool
log_init(void);

//! Check if there's an open log
bool
log_open();

//! Write imc data to the log
bool
log_write(uint8_t* imc_bytes, uint16_t size);

//! Close current log
bool
log_close();

#endif
