#ifndef KEPLER_STM32F767ZI_DISCOVERY_H
#define KEPLER_STM32F767ZI_DISCOVERY_H

#define LED_GREEN_Pin                       GPIO_PIN_0
#define LED_GREEN_GPIO_Port                 GPIOB
#define LED_BLUE_Pin                        GPIO_PIN_7
#define LED_BLUE_GPIO_Port                  GPIOB

#define IMU_DEVICE_NAME_SIZE                16
#define IMU_PORT                            GPIOB
#define IMU_CLK_Pin                         GPIO_PIN_13
#define IMU_MISO_Pin                        GPIO_PIN_14
#define IMU_MOSI_Pin                        GPIO_PIN_15
#define IMU_CS_PORT                         GPIOB
#define IMU_CS_Pin                          GPIO_PIN_12

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

#endif // KEPLER_STM32F767ZI_DISCOVERY_H
