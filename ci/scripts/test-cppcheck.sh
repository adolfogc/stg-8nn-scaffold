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

cd /tmp
export CC=arm-none-eabi-gcc
export ASM=arm-none-eabi-gcc

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON /src

(cppcheck --language=c --error-exitcode=1 --platform=unix32 --std=c99 --project=compile_commands.json)
ec=$?

cppcheck --language=c --error-exitcode=1 --platform=unix32 --std=c99 --dump /src/app/
cppcheck --language=c --error-exitcode=1 --platform=unix32 --std=c99 --dump /src/bsp/
cppcheck --language=c --error-exitcode=1 --platform=unix32 --std=c99  --dump /src/override/dependencies/qpc/ports/arm-cm/qk/gnu/
misra.py --rule-texts=/scripts/misra-c-2012-rule-texts.txt /src/app/*.dump
misra.py --rule-texts=/scripts/misra-c-2012-rule-texts.txt /src/bsp/*.dump
misra.py --rule-texts=/scripts/misra-c-2012-rule-texts.txt /src/override/dependencies/qpc/ports/arm-cm/qk/gnu/*.dump

exit ${ec}
