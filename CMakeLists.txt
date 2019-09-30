include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
SET(CMAKE_CROSSCOMPILING 1)

set(TOOLCHAIN_PREFIX arm-none-eabi)

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}-objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL    ${TOOLCHAIN_PREFIX}-size CACHE INTERNAL "size tool")

cmake_minimum_required(VERSION 3.15)
project(kepler)

enable_language(C ASM)


### Souces and Headers

set(KEPLER_HEADERS
        # This is a bit hacky but need it for config.h
        ${PROJECT_SOURCE_DIR}
        inc)

set(STM_HEADERS
        drivers/STM32F7xx_HAL_Driver/Inc
        drivers/STM32F7xx_HAL_Driver/Inc/Legacy
        drivers/CMSIS/Device/ST/STM32F7xx/Include
        drivers/CMSIS/Include)

set(KEPLER_SOURCES
        startup_stm32f767xx.s
        src/kepler.c
        src/dma.c
        src/gpio.c
        src/icm20948.c
        src/sdmmc.c
        src/spi.c
        src/usart.c
        src/main.c
        src/i2c.c
        src/mpl315a2.c
        src/sdcard/bsp_driver_sd.c
        src/sdcard/diskio.c
        src/sdcard/fatfs.c
        src/sdcard/ff.c
        src/sdcard/ff_gen_drv.c
        src/sdcard/sd_diskio.c
        )

set(STM_SOURCES
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi_ex.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
        drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
        src/stm32f7xx_hal_msp.c
        src/stm32f7xx_it.c
        src/system_stm32f7xx.c)

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
set(LIBS "--specs=rdimon.specs --specs=nano.specs -lgcc -lc -lm -lrdimon")
SET(LDFLAGS "--specs=nosys.specs ${MCU} ${FPU} ${FLOAT-ABI} -T${LDSCRIPT} ${LIBS} -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/kepler.map,--cref -Wl,--gc-sections -u _printf_float -u _sprintf_float"
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
        -f ${PROJECT_SOURCE_DIR}/scripts/kepler.cfg
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