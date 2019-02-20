# Copyright (C) 2018 Adolfo E. García
#
# This file is part of STG-8nn-Scaffold.
#
# STG-8nn-Scaffold is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# STG-8nn-Scaffold is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with STG-8nn-Scaffold.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

project(STG-8nn-Scaffold C ASM)

set(GENERIC_C_FLAGS "-Wall -Wextra -Wredundant-decls -Wmissing-prototypes -Wimplicit-function-declaration -Wshadow -Wno-unused-parameter -Wdouble-promotion -Wswitch-enum -Wfloat-equal -Wconversion -Wtype-limits -Wsign-conversion -Wcast-align -Wmissing-declarations -ffunction-sections -fdata-sections")

if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
  #
  # ARM specific definitions
  #
  add_definitions(-D__ARM_FP=0) # no floating point unit (def. required by QP/C)
  add_definitions(-DSTM32F0)
  add_definitions(-DSTM32F091xC)
  #
  # Flags for use with the GNU ARM Toolchain
  #
  if("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
    set(MCU_FLAGS "-mcpu=cortex-m0 -mtune=cortex-m0 -march=armv6-m -mfloat-abi=soft")
    set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -Wall -fdata-sections -ffunction-sections")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MCU_FLAGS} -mthumb ${GENERIC_C_FLAGS} -Wundef -Og -MMD -MP -MF")
    set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -mthumb --static -specs=nosys.specs -Tstg8nn.ld -Wl,--cref,--gc-sections,-Map=stg8nn.map")
  #
  # Flags for use with Clang's ARM Toolchain
  #
  elseif("${CMAKE_ASM_COMPILER_ID}" MATCHES "Clang")
    set(MCU_FLAGS "--target=armv6m-unknown-none-eabi -march=armv6m -mthumb -mcpu=cortex-m0 -mfloat-abi=soft -mfpu=none")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MCU_FLAGS} -Wno-unused-command-line-argument -ffreestanding ${GENERIC_C_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} --static -specs=nosys.specs -Tstg8nn.ld -Wl,--cref,--gc-sections,-Map=stg8nn.map")
  endif()
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic ${GENERIC_C_FLAGS}")
    set(CMAKE_ASM_FLAGS "-Wall -fdata-sections -ffunction-sections -fno-stack-protector")
    if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
      set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -m32")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo Debug DebugFull Profile MinSizeRel"
      FORCE)
    add_definitions(-DNDEBUG) #qpc,libcanard no debug flag
endif()

set(CMAKE_C_STANDARD 99)

set(BUILD_SHARED_LIBS OFF)

##
## Select model option
##
SET(STG_MODEL "model" CACHE STRING "850")
if("${STG_MODEL}" MATCHES "850")
  message(STATUS "STG MODEL: ${STG_MODEL}")
  add_definitions(-DMODEL_STG850)
elseif("${STG_MODEL}" MATCHES "856")
  message(STATUS "STG MODEL: ${STG_MODEL}")
  add_definitions(-DMODEL_STG856)
elseif("${STG_MODEL}" MATCHES "TESTER")
  message(STATUS "STG MODEL: ${STG_MODEL}")
  add_definitions(-DMODEL_TESTER)
else()
  message(FATAL_ERROR "STG MODEL: invalid option")
endif()

execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE _GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DAPP_SW_GIT_COMMIT_HASH=0x${_GIT_COMMIT_HASH}")

##
## Some diagnostic messages
##
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Git commit hash: 0x${APP_VERSION_GIT_COMMIT_HASH}")

##
## Configure build
##

set(OUTPUT_NAME firmware)

##
## dependencies::qpc(qk)
##

include(CMakeLists_qpc.txt)
list(APPEND SCAFFOLD_SOURCES ${QPC_SOURCES})
list(APPEND SCAFFOLD_INCLUDE ${QPC_INCLUDE})

##
## dependencies::libcanard
##

include(CMakeLists_libcanard.txt)
list(APPEND SCAFFOLD_SOURCES ${CANARD_SOURCES})
list(APPEND SCAFFOLD_INCLUDE ${CANARD_INCLUDE})

##
## uavcan dsdl -> C files
##
include(CMakeLists_uavcan_dsdl.txt)
list(APPEND SCAFFOLD_SOURCES ${DSDL_C_SOURCES})
list(APPEND SCAFFOLD_INCLUDE ${DSDL_C_INCLUDE})

##
## bsp
##
include(CMakeLists_bsp.txt)
list(APPEND SCAFFOLD_SOURCES ${BSP_SOURCES})
list(APPEND SCAFFOLD_INCLUDE ${BSP_INCLUDE})

##
## app
##

include(CMakeLists_app.txt)
list(APPEND SCAFFOLD_SOURCES ${APP_SOURCES})
list(APPEND SCAFFOLD_INCLUDE ${APP_INCLUDE})

##
## elf definition
##

add_executable(${OUTPUT_NAME}.elf ${SCAFFOLD_SOURCES})
target_include_directories(${OUTPUT_NAME}.elf PRIVATE ${SCAFFOLD_INCLUDE})

##
## linker script and bin file
##

if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
  add_custom_command(
    TARGET ${OUTPUT_NAME}.elf PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dependencies/stm32cube/gnu/STM32F091CCUx_FLASH.ld ${PROJECT_BINARY_DIR}/stg8nn.ld
  )

  add_custom_command(TARGET ${OUTPUT_NAME}.elf
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${OUTPUT_NAME}.elf ${OUTPUT_NAME}.bin
  )
endif()

##
## tester libraries
##
if(NOT "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
  target_link_libraries(${OUTPUT_NAME}.elf pthread)
endif()
