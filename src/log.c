//! Kepler includes
#include <config.h>
#include <log.h>
#include <sdcard/fatfs.h>
#include <sdcard/ff.h>
#include <sdmmc.h>

#include <imc-c/LoggingControl.h>

//! HAL includes
#include <inc/debug.h>
#include <inc/led.h>
#include <stm32f7xx_hal.h>
#include <string.h>

// File system
static FATFS fs_obj;
//! Current cache size
static uint16_t cache_curr_size = 0;
//! Total size of current open log
static uint16_t log_total_size = 0;
//! Log cache buffer
static uint8_t bfr_log[LOG_CACHE_CAPACITY];

//! How many logs have been opened/closed since boot
uint16_t log_number = 0;

//! Current 2 digit log number + log name + extension + \0
char log_name[13];

bool log_is_open = false;

//! Currently open log
FIL log_fd;

//! IMC Log control
LoggingControl imc_logc;


/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  SDCARD_DMA_RCC_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(SDCARD_DMA_STREAM_X, 0, 0);
  HAL_NVIC_EnableIRQ(SDCARD_DMA_STREAM_X);

  HAL_NVIC_SetPriority(SDCARD_DMA_STREAM_Y, 0, 0);
  HAL_NVIC_EnableIRQ(SDCARD_DMA_STREAM_Y);
}

FRESULT do_write(const uint8_t* bfr, uint16_t size)
{
  if (bfr == NULL)
    return false;

  if (size == 0)
    return true;

  FRESULT ret;
  unsigned cursor;
  do
  {
    ret = f_write(&log_fd, bfr, size, &cursor);
  } while(ret == FR_OK && cursor < size);

  return ret;
}

// Register, using IMC, a log operation
static void
logcontrol_toggle(bool starting)
{
  if (starting)
    imc_logc.op = COP_REQUEST_START;
  else
    imc_logc.op = COP_REQUEST_STOP;

  size_t serialization_size = LoggingControl_serialization_size(&imc_logc);
  LoggingControl_serialize(&imc_logc, bfr_log);
  cache_curr_size = serialization_size;
  log_total_size = serialization_size;
}

bool
log_init(void)
{
  MX_FATFS_Init();
  HAL_Delay(10);

  imc_logc = LoggingControl_new();
  return f_mount(&fs_obj, "", 1) == FR_OK;
}

void
log_update()
{
  if (log_size() >= SDCARD_LOG_SIZE)
  {
    led_on(BLUE);
    trace("log rotation\r\n");
    if (!log_close())
      trace("failed to close\r\n");

    if (!log_open())
      trace("failed to rotate log\r\n");
    led_off(BLUE);
  }
}

uint16_t
log_size()
{
  if (!log_is_open)
    return 0;

  return log_total_size;
}

// TODO create log with proper names
bool
log_open()
{
  if (log_is_open)
    return false;

  imc_logc.name[0] = 0;

  unsigned n = sprintf(&imc_logc.name[0], "l%d", log_number);
  strncat(&log_name[0], &imc_logc.name[0], n);
  strcat(log_name, ".lsf");
  FRESULT res = f_open(&log_fd, log_name, FA_CREATE_ALWAYS | FA_WRITE);

  if (res != FR_OK)
  {
    log_name[0] = 0;
    return false;
  }

  log_total_size = 0;
  cache_curr_size = 0;
  bfr_log[0] = 0;
  log_is_open = true;

  logcontrol_toggle(true);

  return true;
}

bool
log_write(uint8_t* imc_bytes, uint16_t size)
{
  if (!log_is_open)
    return false;

  log_total_size += size;

  uint8_t* __ptr = imc_bytes;
  // cache is/will be full, then write to disk
  if (cache_curr_size + size > LOG_CACHE_CAPACITY)
  {
    unsigned diff = LOG_CACHE_CAPACITY - cache_curr_size;
    memcpy(&bfr_log[cache_curr_size], &imc_bytes, diff);
    if (do_write(bfr_log, LOG_CACHE_CAPACITY) != FR_OK)
      return false;

    cache_curr_size = 0;
    __ptr += diff;
    size -= diff;
  }

  if (size == 0)
    return false;

  // cache data
  memcpy(&bfr_log[cache_curr_size], &__ptr, size);
  cache_curr_size += size;

  return true;
}

bool
log_close()
{
  log_total_size = 0;
  if (!log_is_open)
    return false;

  do_write(bfr_log, cache_curr_size);
  log_is_open = false;
  ++log_number;
  log_name[0] = 0;
  imc_logc.name[0] = 0;

  return f_close(&log_fd) == FR_OK;
}
