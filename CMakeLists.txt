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
set(CMAKE_TOOLCHAIN_FILE stm32f091xc-toolchain.cmake)

project(STG-8nn-Scaffold C ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo Debug DebugFull Profile MinSizeRel"
      FORCE)
    add_definitions(-DNDEBUG) #qpc no debug flag
endif()

set(BUILD_SHARED_LIBS OFF)

##
## dependencies::libopencm3
##
include(CMakeLists-libopencm3.txt)

##
## dependencies::qpc(qk)
##
include(CMakeLists-qpc.txt)

##
## bsp
##
include(CMakeLists-bsp.txt)

##
## app
##
include(CMakeLists-app.txt)
