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

# Using this toolchain for macOS: https://github.com/eblot/homebrew-armeabi

cmake_minimum_required(VERSION 3.5)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING 1)

set(triple armv6m-none-eabi)

set(CMAKE_C_COMPILER /usr/local/opt/arm-none-eabi-llvm/bin/clang)
set(CMAKE_C_COMPILER_TARGET ${triple})

set(CMAKE_CXX_COMPILER /usr/local/opt/arm-none-eabi-llvm/bin/clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

set(CMAKE_ASM_COMPILER /usr/local/opt/arm-none-eabi-llvm/bin/clang)

set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} --target=${triple} -fuse-ld=lld -L/usr/local/opt/armv6m-cortex-m0plus/armv6m-none-eabi/cortex-m0plus/lib")

# In order to pass CMake compilation test (compiler works):
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH /usr/local/opt/armv6m-cortex-m0plus/armv6m-none-eabi/cortex-m0plus)
include_directories(/usr/local/opt/armv6m-cortex-m0plus/armv6m-none-eabi/cortex-m0plus/include)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
