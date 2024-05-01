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

cmake_minimum_required(VERSION 3.20)

project(STG-8nn-Scaffold C ASM) # it's important to select both C and ASM here

set(CMAKE_C_STANDARD 99)

##
## Add dependency: STG-8nn Scaffold
##
include(CMakeLists_scaffold.txt)

##
## ELF file
##
set(OUTPUT_NAME firmware)
add_executable(${OUTPUT_NAME}.elf ${SCAFFOLD_SOURCES})
target_include_directories(${OUTPUT_NAME}.elf PRIVATE ${SCAFFOLD_INCLUDE})

##
## Platform specific commands
##
if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
  ###
  ### ARM
  ###

  ### BIN file
  add_custom_command(TARGET ${OUTPUT_NAME}.elf
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} --strip-debug --strip-unneeded -O binary -S ${OUTPUT_NAME}.elf ${OUTPUT_NAME}.bin && (du -h ${OUTPUT_NAME}.bin | cut -f1)
  )
else()
  ###
  ### x86
  ###

  ### Link to PThreads library
  target_link_libraries(${OUTPUT_NAME}.elf pthread)
endif()
