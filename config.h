#ifndef __KEPLER_CONFIG_H__
#define __KEPLER_CONFIG_H__

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

#endif
