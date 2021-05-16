###########################################
# BSP for STM32F767zi-Nucleo44            #
# Currently it only compiles mock drivers #
###########################################

# All BSP sources
set(BSP_SOURCES
        ${BSP_SOURCE_DIR}/bsp.cpp
        ${CMAKE_SOURCE_DIR}/src/driver/mock/GPS.hpp
        ${CMAKE_SOURCE_DIR}/src/driver/mock/IMU.hpp
        ${CMAKE_SOURCE_DIR}/src/driver/mock/Altimeter.hpp)

# Path to Config.hpp file
set(BSP_CONFIG_HPP ${BSP_SOURCE_DIR}/Config.hpp)

# BSP's include directories
set(BSP_INCDIR
        ${CMAKE_SOURCE_DIR}/bsp/${BSP})

# Configuration file to flash firmware with OpenOCD
set(BSP_FLASH_CONFIG ${BSP_SOURCE_DIR}/kepler.cfg)