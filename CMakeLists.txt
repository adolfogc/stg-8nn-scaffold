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

add_definitions(-D__ARM_FP=0) # no floating point unit (def. required by QP/C)
add_definitions(-DSTM32F0)
add_definitions(-DSTM32F091xC)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set(MCU_FLAGS "-mcpu=cortex-m0 -mtune=cortex-m0 -march=armv6-m -mfloat-abi=soft")
  set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -Wall -fdata-sections -ffunction-sections")
  set(CMAKE_C_FLAGS "${MCU_FLAGS} -mthumb -Wall -Wextra -Wredundant-decls -Wmissing-prototypes -Wimplicit-function-declaration -Wundef -Wshadow -Wno-unused-parameter -ffunction-sections -fdata-sections -Og -MMD -MP -MF")
  set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -mthumb -specs=nano.specs -lc -lm -lnosys -Tstg8nn.ld -Wl,--cref,--gc-sections,-Map=stg8nn.map")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo Debug DebugFull Profile MinSizeRel"
      FORCE)
    add_definitions(-DNDEBUG) #qpc no debug flag
endif()

set(CMAKE_C_STANDARD 99)

set(BUILD_SHARED_LIBS OFF)

##
## Configure build
##

set(STG8NN_ELF firmware.elf)
set(STG8NN_BIN firmware.bin)

##
## dependencies::stm32cube
##

include(CMakeLists_stm32cube.txt)
list(APPEND STG8NN_SOURCES ${STM32CUBE_SOURCES})
list(APPEND STG8NN_INCLUDE ${STM32CUBE_INCLUDE})

##
## dependencies::qpc(qk)
##

include(CMakeLists_qpc.txt)
list(APPEND STG8NN_SOURCES ${QPC_SOURCES})
list(APPEND STG8NN_INCLUDE ${QPC_INCLUDE})

##
## bsp
##

include(CMakeLists_bsp.txt)
list(APPEND STG8NN_SOURCES ${BSP_SOURCES})
list(APPEND STG8NN_INCLUDE ${BSP_INCLUDE})

##
## app
##
include(CMakeLists_app.txt)
list(APPEND STG8NN_SOURCES ${APP_SOURCES})
list(APPEND STG8NN_INCLUDE ${APP_INCLUDE})

##
## elf definition
##

add_executable(${STG8NN_ELF} ${STG8NN_SOURCES})
target_include_directories(${STG8NN_ELF} PRIVATE ${STG8NN_INCLUDE})

##
## linker script and bin file
##

add_custom_command(
  TARGET ${STG8NN_ELF} PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dependencies/stm32cube/gnu/STM32F091CCUx_FLASH.ld ${PROJECT_BINARY_DIR}/stg8nn.ld
)

add_custom_command(TARGET ${STG8NN_ELF}
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary -S ${STG8NN_ELF} ${STG8NN_BIN}
)