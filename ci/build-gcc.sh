#!/bin/sh

cd /build && \
export CC=arm-none-eabi-gcc && \
cmake -GNinja /src && \
cmake --build . && \
size firmware.elf && \
echo "firmware.bin size: `du -h firmware.bin | cut -f1`"
