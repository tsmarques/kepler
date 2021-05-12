include(cmake/Toolchain.cmake)

cmake_minimum_required(VERSION 3.19)
project(kepler)

set(CMAKE_CXX_STANDARD 17)
enable_language(C CXX ASM)

macro(kepler_target target cpp_extra)
    add_executable(${target} ${KEPLER_SOURCES} ${CHIBIOS_SOURCES} ${cpp_extra})
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${CHIBIOS_CFLAGS}>)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C>:${CHIBIOS_CFLAGS}>)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CHIBIOS_CPPFLAGS}>)
    target_include_directories(${target} PRIVATE ${KEPLER_INCDIR} ${CHIBIOS_INCDIR})
    target_link_libraries(${target} PRIVATE "${CHIBIOS_LDFLAGS} -Wl,-Map=${target}.map")
    set_target_properties(${target} PROPERTIES SUFFIX .elf)

    # hex
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ihex ${target}.elf ${target}.hex)

    # bin
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O binary ${target}.elf ${target}.bin)

    # dump assembly
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_OBJDUMP} -x --syms ${target}.elf > ${target}.dmp
            VERBATIM)

    # assembly + code
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_OBJDUMP} -S ${target}.elf > ${target}.list
            VERBATIM)

    # elf size
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_SIZE_UTIL} ${target}.elf)

    # flash
    add_custom_target(${target}-flash
            openocd
            -f ${BSP_FLASH_CONFIG}
            -c init
            -c targets
            -c "arm semihosting enable"
            -c "arm semihosting_fileio disable"
            -c "reset init"
            -c "flash write_image erase ${CMAKE_BINARY_DIR}/${target}.elf"
            -c "verify_image ${CMAKE_BINARY_DIR}/${target}.elf"
            -c "reset run"
            DEPENDS ${target}
            )

endmacro()

if (NOT BSP)
    set(BSP "stm32f767zi-nucleo44")
endif()

message(STATUS "BSP: ${BSP}")
set(BSP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/bsp/${BSP})
set(BSP_CMAKE_FILE ${BSP_SOURCE_DIR}/bsp.cmake)

if (NOT EXISTS "${BSP_CMAKE_FILE}")
    message(FATAL_ERROR "File ${BSP_CMAKE_FILE} not found\nCheck ${CMAKE_SOURCE_DIR}/bsp for available BSPs")
endif()

include(cmake/ChibiOS.cmake)
include(${BSP_CMAKE_FILE})

set(KEPLER_INCDIR src ${BSP_INCDIR})

set(KEPLER_SOURCES
        ${BSP_CONFIG_HPP}
        ${BSP_SOURCES}
        bsp/trace.h
        src/kepler.hpp
        src/data/BasicData.hpp
        src/data/GpsFix.hpp
        src/data/Attitude.hpp
        src/data/Pressure.hpp)

kepler_target(kepler src/main.cpp)
target_compile_options(kepler PRIVATE -O2)

kepler_target(kepler-dbg src/main.cpp)
target_compile_options(kepler-dbg PRIVATE -ggdb)
target_compile_definitions(kepler-dbg PRIVATE -DARM_SEMIHOST)
target_link_libraries(kepler-dbg PRIVATE --specs=rdimon.specs -lrdimon -lc)
