macro(chibios_extract VAR_NAME _AS_LIST)
    execute_process(
            COMMAND make TRGT=${TOOLCHAIN_PREFIX} print-${VAR_NAME}
            COMMAND_ECHO STDOUT
            ECHO_ERROR_VARIABLE
            COMMAND_ERROR_IS_FATAL ANY
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE CHIBIOS_${VAR_NAME})

    string(REGEX REPLACE ".dep/print-C[P]*FLAGS.d" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-Wa,-alms=build/lst/" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-O0" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-O1" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-O2" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-O3" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-Os" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-ggdb" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    string(REGEX REPLACE "-MD -MP -MF" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")

    string(REGEX REPLACE "\n$" "" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    if (${_AS_LIST})
        string(REPLACE " " ";" CHIBIOS_${VAR_NAME} "${CHIBIOS_${VAR_NAME}}")
    endif()
endmacro()


# CHIBIOS_CSRC
chibios_extract(CSRC 1)
## CHIBIOS_CPPSRC
chibios_extract(CPPSRC 1)
## CHIBIOS_INCDIR
chibios_extract(INCDIR 1)
## CHIBIOS_CFLAGS
chibios_extract(CFLAGS 1)
## CHIBIOS_CPPFLAGS
chibios_extract(CPPFLAGS 1)
## CHIBIOS_ASMSCR
chibios_extract(ASMSRC 1)
## CHIBIOS_ASMXSRC
chibios_extract(ASMXSRC 1)
# CHIBIOS_LDFLAGS
#chibios_get(LDFLAGS)

set(CHIBIOS_LDFLAGS "-mcpu=cortex-m7 -fomit-frame-pointer -falign-functions=16 -Wno-psabi -ffunction-sections -fdata-sections -fno-common -flto -mfloat-abi=hard -mfpu=fpv5-d16 -nostartfiles  -Wl,--cref,--no-warn-mismatch,--library-path=${CMAKE_SOURCE_DIR}/vendor/chibios/os/common/startup/ARMCMx/compilers/GCC/ld,--script=${CMAKE_SOURCE_DIR}/vendor/chibios/os/common/startup/ARMCMx/compilers/GCC/ld/STM32F76xxI.ld,--gc-sections,--defsym=__process_stack_size__=0x500,--defsym=__main_stack_size__=0x500")
#

set(CHIBIOS_SOURCES
        ${CHIBIOS_CSRC}
        ${CHIBIOS_CPPCSRC}
        ${CHIBIOS_ASMSRC}
        ${CHIBIOS_ASMXSRC})


message(STATUS "CHIBIOS SRC:" ${CHIBIOS_SOURCES})
message(STATUS "CFLAGS: ${CHIBIOS_CFLAGS}")
message(STATUS "CPPFLAGS: ${CHIBIOS_CPPFLAGS}")
message(STATUS "LDFLAGS: ${CHIBIOS_LDFLAGS}")