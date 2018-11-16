#!/bin/sh

cd /src && \
cppcheck --std=c89 --platform=unix32 --language=c --error-exitcode=1 app && \
cppcheck --std=c89 --platform=unix32 --language=c --error-exitcode=1 bsp && \
cppcheck --std=c89 --platform=unix32 --language=c --error-exitcode=1 override/dependencies/qpc
