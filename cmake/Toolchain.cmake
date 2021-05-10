set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# https://cmake.org/pipermail/cmake-developers/2016-February/027871.html
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


string(REPLACE "gcc" "" TOOLCHAIN_PREFIX ${CMAKE_C_COMPILER})

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL    ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

message("-- toolchain-prefix: ${TOOLCHAIN_PREFIX}")
message("-- c compiler:       ${CMAKE_C_COMPILER}")
message("-- cpp compiler:     ${CMAKE_CXX_COMPILER}")
message("-- objcopy:          ${CMAKE_OBJCOPY}")
message("-- size:             ${CMAKE_SIZE_UTIL}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
