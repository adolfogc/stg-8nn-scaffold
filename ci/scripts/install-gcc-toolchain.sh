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

if [ ! -d ${HOME}/.cache/gcc-arm-none-eabi ]; then
  curl -SOL https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2018q2/gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 && \
  tar xjf gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 && \
  mkdir -p ${HOME}/.cache/gcc-arm-none-eabi && \
  cp -r ./gcc-arm-none-eabi-7-2018-q2-update/* ${HOME}/.cache/gcc-arm-none-eabi
fi

export PATH="${HOME}/.cache/gcc-arm-none-eabi/bin:${PATH}"
