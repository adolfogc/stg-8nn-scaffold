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

##
## Scaffold include file
##
include(CMakeLists_scaffold.txt.in)

##
## Configure build
##

set(OUTPUT_NAME firmware)

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
