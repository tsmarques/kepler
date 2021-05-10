include(cmake/Toolchain.cmake)

cmake_minimum_required(VERSION 3.19)
project(kepler)

set(CMAKE_CXX_STANDARD 17)
enable_language(C CXX ASM)
include(cmake/ChibiOS.cmake)


macro(kepler_target target cpp_extra)
    add_executable(${target} ${CHIBIOS_SOURCES} ${cpp_extra})
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${CHIBIOS_CFLAGS}>)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:C>:${CHIBIOS_CFLAGS}>)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CHIBIOS_CPPFLAGS}>)
    target_include_directories(${target} PRIVATE ${KEPLER_INCDIR} ${CHIBIOS_INCDIR})
    target_link_libraries(${target} PRIVATE "${CHIBIOS_LDFLAGS} -Wl,-Map=${target}.map")
    set_target_properties(${target} PROPERTIES SUFFIX .elf)

    # Flash.
    add_custom_target(${target}-flash
            openocd
            -f ${CMAKE_SOURCE_DIR}/kepler.cfg
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

set(KEPLER_INCDIR src bsp)

set(KEPLER_SOURCES
        src/main.cpp
        src/data/BasicData.hpp
        src/data/Attitude.hpp)

kepler_target(kepler src/main.cpp)
target_compile_options(kepler PRIVATE -O2)

kepler_target(kepler-dbg src/main.cpp)
target_compile_options(kepler-dbg PRIVATE -ggdb)
target_compile_definitions(kepler-dbg PRIVATE -DARM_SEMIHOST)
target_link_libraries(kepler-dbg PRIVATE --specs=rdimon.specs -lrdimon -lc)
