#ifndef __KEPLER_CONFIG_H__
#define __KEPLER_CONFIG_H__

#ifndef KEPLER_STM32F767_TESTBOARD_1_H
#define KEPLER_STM32F767_TESTBOARD_1_H

#define PORT_MAIN_POWER                     (GPIOE)
#define PIN_MAIN_POWER                      (GPIO_PIN_10)

//! Debug Configs
#define DEBUG_SERIAL                        (UART4)
#define DEBUG_SERIAL_BAUDRATE               (921600)

//! GPS
#define GPS_SERIAL                          (USART2)
#define GPS_BAUDRATE                        (460800)
#define GPS_PORT_PPS                        (GPIOD)
#define GPS_PIN_PPS                         (GPIO_PIN_4)
#define GPS_PORT_POWER                      (GPIOD)
#define GPS_PIN_POWER                       (GPIO_PIN_7)
#define GPS_MAX_NUMBER_BUF_RX               (5)
#define GPS_MIN_SAT_TO_FIX                  (4)
#define GPS_MIN_FIX_HDOP                    (10.0)

//! WIFI
#define WIFI_SERIAL                         (USART6)
#define WIFI_PRE_CONFIG                     (false)
#define WIFI_SERIAL_BAUDRATE                (2304000)
#define PORT_WIFI_POWER                     (GPIOB)
#define PIN_WIFI_POWER                      (GPIO_PIN_3)
#define PORT_WIFI_RESET                     (GPIOD)
#define PIN_WIFI_RESET                      (GPIO_PIN_15)
#define WIFI_SPEW_RSP                       (false)
#define WIFI_SPEW_DEBUG                     (false)
#define WIFI_TIMEOUT                        (6)
#define WIFI_TCP_TIMEOUT                    (10)
#define WIFI_SPEW_SDK_VERSION               (false)

//! IMU
#define IMU_DEVICE_NAME_SIZE                16
#define IMU_SPI_DEVICE                      SPI2
#define IMU_PORT_POWER                      GPIOD
#define IMU_PIN_POWER                       GPIO_PIN_8
#define IMU_PORT                            GPIOB
#define IMU_CLK_Pin                         GPIO_PIN_13
#define IMU_MISO_Pin                        GPIO_PIN_14
#define IMU_MOSI_Pin                        GPIO_PIN_15
#define IMU_CS_PORT                         GPIOB
#define IMU_CS_Pin                          GPIO_PIN_12
#define IMU_DEFAULT_SAMPLES_PER_S           (1)
#define IMU_RCC_ENABLE __HAL_RCC_SPI2_CLK_ENABLE
#define IMU_GPIO_CLOCK_ENABLE               __HAL_RCC_GPIOB_CLK_ENABLE
#define IMU_IRQN                            SPI2_IRQn
#define IMU_RCC_DISABLE                     __HAL_RCC_SPI2_CLK_DISABLE
#define IMU_SPI_HANDLE                      hspi2


//! EEPROM
#define EEPROM                              (I2C1)
#define EEPROM_DEV_ADDR                     (0x50)
#define EEPROM_MAX_SIZE                     (0x0100) //256 - 6 bytes (3ms for 1 byte)
#define EEPROM_SPEW_CONFIG                  (false)

//! SD Card
#define SDCARD_DEVICE                       SDMMC1
#define SDCARD_DMA_RCC_ENABLE               __HAL_RCC_DMA2_CLK_ENABLE
#define SDCARD_DMA_STREAM_X                 DMA2_Stream3_IRQn
#define SDCARD_DMA_STREAM_Y                 DMA2_Stream6_IRQn
#define SDCARD_GPIO                         GPIOC
#define SDCARD_D0_Pin                       GPIO_PIN_8
#define SDCARD_D1_Pin                       GPIO_PIN_9
#define SDCARD_D2_Pin                       GPIO_PIN_10
#define SDCARD_D3_Pin                       GPIO_PIN_11
#define SDCARD_CK_Pin                       GPIO_PIN_12
#define SDCARD_CMD_Pin                      GPIO_PIN_2
#define SDCARD_CMD_GPIO_Port                GPIOD
#define SDCARD_PORT_POWER                   (GPIOD)
#define SDCARD_PIN_POWER                    (GPIO_PIN_0)

//! SWITCH HALL
#define PORT_SWITCH_HAL                     (GPIOE)
#define PIN_SWITCH_HAL                      (GPIO_PIN_12)
#define DELAY_IN_READINGS                   (100) //ms
#define NUMBER_SAMPLES_TO_VALIDATE_MODE     (20)
#define NUMBER_SAMPLES_TO_VALIDATE_MISSION  (20)
#define NUMBER_SAMPLES_TO_VALIDATE_POWEROFF (50)

//! RTC
#define RV8523_RTC                          (I2C1)
#define RV8526_RTC_ADDR                     (0xD0>>1)
#define RV8526_RTC_SPEW_DEBUG               (false)

//! TERMISTOR
#define TERMISTOR                           (I2C1)
#define ADS1115_ADDRESS                     (0x48)    // 1001 000 (ADDR = GND)
#define TERMISTOR_PORT_POWER                (GPIOB)
#define TERMISTOR_PIN_POWER                 (GPIO_PIN_9)
#define ADS_SPEW_VOLT                       (false)

//! BATTERY
#define ADC_SOURCE                          (ADC1)
#define ADC_BATTERY_GPIO_PORT               (GPIOA)
#define ADC_BATTERY_PIN                     (GPIO_PIN_6)
#define ADC_BATTERY_POWER_PORT              (GPIOA)
#define ADC_BATTERY_POWER_PIN               (GPIO_PIN_5)
#define PORT_BATTERY_STATE                  (GPIOE)
#define PIN_BATTERY_STATE                   (GPIO_PIN_9)
#define SPEW_ADC_VALUE                      (false)
#define ADC_PERIOD_READ                     (5)//seconds

//! LED's
#define LED_RED_Pin                         GPIO_PIN_1
#define LED_RED_GPIO_Port                   GPIOA
#define LED_BLUE_GPIO_Port                  GPIOA
#define LED_GREEN_Pin                       GPIO_PIN_3
#define LED_GREEN_GPIO_Port                 GPIOA
#define LED_BLUE_Pin                        GPIO_PIN_2
#define LED_BLUE_GPIO_Port                  GPIOA

//! Altimeter I2C's SDA pin
#define MPL_SDA                             GPIO_PIN_13
//! Altimeter I2C's SCL pin
#define MPL_SCL                             GPIO_PIN_12
//! Altimeter's  I2C device
#define MPL_I2C_DEVICE                      I2C4
//! Altimeter's GPIO
#define MPL_I2C_GPIO                        GPIOD
//! Altimeter's I2C Slave Address
#define MPL_I2C_ADDR                        0x60

//! How many bytes to accumulate before writing to disk
#define LOG_CACHE_CAPACITY                  1024

#endif

#endif
