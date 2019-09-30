//! Kepler includes
#include <config.h>
#include <log.h>
#include <sdcard/fatfs.h>
#include <sdcard/ff.h>
#include <sdmmc.h>

//! HAL includes
#include <stm32f7xx_hal.h>
#include <string.h>

//! SDMMC handler
SD_HandleTypeDef hsd1;
//! SDMMC's DMA RX Handler
DMA_HandleTypeDef hdma_sdmmc1_rx;
//! SDMMC's DMA TX Handler
DMA_HandleTypeDef hdma_sdmmc1_tx;

// File system
static FATFS fs_obj;
//! Current cache size
static uint16_t log_curr_size = 0;
//! Log cache buffer
static uint8_t bfr_log[LOG_CACHE_CAPACITY];

//! Currently open log
FIL log_fd;

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

FRESULT do_write(const uint8_t* bfr, uint16_t size)
{
  FRESULT ret;
  unsigned cursor;
  do {
    ret = f_write(&log_fd, bfr, size, &cursor);
  } while(ret == FR_OK && cursor < size);

  return ret;
}

void log_init(void)
{
  MX_DMA_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
}

// TODO create log with proper names
bool
log_open()
{
  FRESULT res = f_open(&log_fd, "log.lsf", FA_CREATE_ALWAYS | FA_WRITE);

  if (res != FR_OK)
    return false;

  log_curr_size = 0;
  bfr_log[0] = 0;

  return true;
}

bool
log_write(uint8_t* imc_bytes, uint16_t size)
{
  uint8_t* __ptr = imc_bytes;
  // cache is/will be full, then write to disk
  if (log_curr_size + size > LOG_CACHE_CAPACITY)
  {
    unsigned diff = LOG_CACHE_CAPACITY - log_curr_size;
    memcpy(&bfr_log[log_curr_size], &imc_bytes, diff);
    if (do_write(bfr_log, LOG_CACHE_CAPACITY) != FR_OK)
      return false;

    __ptr += diff;
    size -= diff;
  }

  if (size == 0)
    return false;

  // cache data
  memcpy(&bfr_log[log_curr_size], &__ptr, size);

  return true;
}

bool
log_close()
{
  return f_close(&log_fd) == FR_OK;
}
