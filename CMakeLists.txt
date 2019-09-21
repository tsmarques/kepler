cmake_minimum_required(VERSION 3.15)
project(kepler)

set(CMAKE_CXX_STANDARD 14)

set(KEPLER_HEADERS Inc)
set(STM_HEADERS Drivers/STM32F7xx_HAL_Driver/Inc)

set(KEPLER_SOURCES
        Src/ctb.c
        Src/dma.c
        Src/gpio.c
        Src/icm20948.c
        Src/sdmmc.c
        Src/spi.c
        Src/usart.c)

set(STM_SOURCES
        Src/stm32f7xx_hal_msp.c
        Src/stm32f7xx_it.c
        Src/system_stm32f7xx.c)

add_executable(kepler config.h ${KEPLER_SOURCES} ${STM_SOURCES})
target_include_directories(kepler PRIVATE ${KEPLER_HEADERS} ${STM_HEADERS})