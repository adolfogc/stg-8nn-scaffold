#!/bin/sh

# Copyright (C) 2018 Adolfo E. García

# This file is part of STG-8nn-Scaffold.

# STG-8nn-Scaffold is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# STG-8nn-Scaffold is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.

# You should have received a copy of the GNU Affero General Public License
# along with STG-8nn-Scaffold.  If not, see <www.gnu.org/licenses/>.

wrkdir=${PWD}

(mkdir -p build && cd build && \
cmake -DSTG_MODEL:STRING=856 -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake ${wrkdir} && \
build-wrapper-linux-x86-64 --out-dir bw-output make && \
cd .. && sonar-scanner)
