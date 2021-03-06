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

mkdir -p build && cd build

cmake -DSTG_MODEL:STRING=856 -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ${wrkdir}

cppcheck --language=c --error-exitcode=1 --platform=unix32 --std=c99 --dump ${wrkdir}/app/
cppcheck --language=c --error-exitcode=1 --platform=unix32 --std=c99 --dump ${wrkdir}/bsp/
#cppcheck --language=c --error-exitcode=1 --platform=unix32 --std=c99 --dump ${wrkdir}/dependencies/stm32cube

if [ -e ${wrkdir}/ci/scripts/misra-c-2012-rule-texts.txt ]
then
  export MISRA_RULES_TEXT="--rule-texts=${wrkdir}/ci/scripts/misra-c-2012-rule-texts.txt"
fi

eval "misra.py ${MISRA_RULES_TEXT} ${wrkdir}/app/*.dump"
ec=${ec} && $?

eval "misra.py ${MISRA_RULES_TEXT} ${wrkdir}/bsp/*.dump"
ec=${ec} && $?

# MISRA C:2012 checks are not enforced for files in these directories:
#(eval "misra.py ${MISRA_RULES_TEXT} ${wrkdir}/dependencies/stm32cube/*.dump")

cd ${wrkdir}
rm -rf build
exit ${ec}
