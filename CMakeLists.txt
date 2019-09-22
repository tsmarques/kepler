cmake_minimum_required(VERSION 3.15)
project(kepler)

enable_language(C ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN_PREFIX arm-none-eabi)

# https://cmake.org/pipermail/cmake-developers/2016-February/027871.html
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}-objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL    ${TOOLCHAIN_PREFIX}-size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

### Souces and Headers

set(KEPLER_HEADERS
        # This is a bit hacky but need it for config.h
        ${PROJECT_SOURCE_DIR}
        Inc)

set(STM_HEADERS
        Drivers/STM32F7xx_HAL_Driver/Inc
        Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
        Drivers/CMSIS/Device/ST/STM32F7xx/Include
        Drivers/CMSIS/Include)

set(KEPLER_SOURCES
        startup_stm32f767xx.s
        Src/kepler.c
        Src/dma.c
        Src/gpio.c
        Src/icm20948.c
        Src/sdmmc.c
        Src/spi.c
        Src/usart.c
        Src/main.c
        Src/i2c.c
        Src/mpl315a2.c)

set(STM_SOURCES
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi_ex.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
        Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
        Src/stm32f7xx_hal_msp.c
        Src/stm32f7xx_it.c
        Src/system_stm32f7xx.c)

### Flags, Options and Linker

# MCU
set(CPU "-mcpu=cortex-m7")
set(FPU "-mfpu=fpv5-d16")
set(FLOAT-ABI "-mfloat-abi=hard")
set(MCU "${CPU} -mthumb -mlittle-endian -mthumb-interwork -std=gnu99")

# GCC
set(OPT "-O3")
SET(CMAKE_C_FLAGS "${MCU} -fno-builtin ${FPU} ${FLOAT-ABI} -Wall ${AS_INCLUDES} ${OPT} -ffunction-sections -fdata-sections"
        CACHE INTERNAL "c compiler flags")

# Assembly
set(ASFLAGS "${MCU} ${AS_DEFS} ${AS_INCLUDES} ${OPT} -Wall -fdata-sections -ffunction-sections -std=gnu99")
SET(CMAKE_ASM_FLAGS "-x assembler-with-cpp ${ASFLAGS}"
        CACHE INTERNAL "asm compiler flags")

# C defines
add_definitions(
        "-DUSE_HAL_DRIVER"
        "-DSTM32F767xx"
)

set(LDSCRIPT "${PROJECT_SOURCE_DIR}/STM32F767VITx_FLASH.ld")
set(LIBS "-lc -lm --specs=rdimon.specs")
SET(LDFLAGS "${MCU} ${FPU} ${FLOAT-ABI} -T${LDSCRIPT} -specs=nano.specs ${LIBS} -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/kepler.map,--cref -Wl,--gc-sections -u _printf_float -u _sprintf_float"
        CACHE INTERNAL "executable linker flags")

### Targets

add_executable(kepler config.h ${KEPLER_SOURCES} ${STM_SOURCES})
target_include_directories(kepler PRIVATE ${KEPLER_HEADERS} ${STM_HEADERS})
target_link_libraries(kepler PRIVATE "${LDFLAGS}")
set_target_properties(kepler PROPERTIES SUFFIX .elf)

# iHEX
add_custom_command(TARGET kepler POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex kepler.elf kepler.hex
        )

# Raw
add_custom_command(TARGET kepler POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary kepler.elf kepler.bin
        )

# Size.
add_custom_command(TARGET kepler POST_BUILD
        COMMAND arm-none-eabi-size kepler.elf)

add_custom_target(kepler-flash
        openocd
        -f board/stm32f7discovery.cfg
        -c init
        -c targets
        -c "arm semihosting enable"
        -c "arm semihosting_fileio disable"
        -c "reset init"
        -c "flash write_image erase ${CMAKE_CURRENT_BINARY_DIR}/kepler.elf"
        -c "verify_image ${CMAKE_CURRENT_BINARY_DIR}/kepler.elf"
        -c "reset run"
        DEPENDS kepler
)