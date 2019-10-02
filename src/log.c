//! Kepler includes
#include <config.h>
#include <log.h>
#include <sdcard/fatfs.h>
#include <sdcard/ff.h>
#include <sdmmc.h>

//! HAL includes
#include <stm32f7xx_hal.h>
#include <string.h>

// File system
static FATFS fs_obj;
//! Current cache size
static uint16_t log_curr_size = 0;
//! Log cache buffer
static uint8_t bfr_log[LOG_CACHE_CAPACITY];

bool log_is_open = false;

//! Currently open log
FIL log_fd;

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

bool
log_init(void)
{
  MX_DMA_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  return f_mount(&fs_obj, "", 1) == FR_OK;
}

// TODO create log with proper names
bool
log_open()
{
  if (log_is_open)
    return false;

  FRESULT res = f_open(&log_fd, "log.lsf", FA_CREATE_ALWAYS | FA_WRITE);

  if (res != FR_OK)
    return false;

  log_curr_size = 0;
  bfr_log[0] = 0;
  log_is_open = true;

  return true;
}

bool
log_write(uint8_t* imc_bytes, uint16_t size)
{
  if (!log_is_open)
    return false;

  uint8_t* __ptr = imc_bytes;
  // cache is/will be full, then write to disk
  if (log_curr_size + size > LOG_CACHE_CAPACITY)
  {
    unsigned diff = LOG_CACHE_CAPACITY - log_curr_size;
    memcpy(&bfr_log[log_curr_size], &imc_bytes, diff);
    if (do_write(bfr_log, LOG_CACHE_CAPACITY) != FR_OK)
      return false;

    log_curr_size = 0;
    __ptr += diff;
    size -= diff;
  }

  if (size == 0)
    return false;

  // cache data
  memcpy(&bfr_log[log_curr_size], &__ptr, size);
  log_curr_size += size;

  return true;
}

bool
log_close()
{
  if (!log_is_open)
    return false;

  do_write(bfr_log, log_curr_size);
  log_is_open = false;
  return f_close(&log_fd) == FR_OK;
}
