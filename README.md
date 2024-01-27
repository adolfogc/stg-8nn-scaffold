# STG-8nn-Scaffold

<img align="right" width="80" height="100" src="stg-8nn-scaffold-logo.png">

A base project for programming the [BARTH® STG-8nn mini-PLCs](https://barth-elektronik.com/en/mini-plc.html).

Note (2024): this project will no longer be updated.

## Motivation
This project serves as a scaffold to make firmware in C for the BARTH® STG-8nn mini-PLCs, using macOS or Linux + Docker as the development platform. For an example on how to use this project, refer to the one provided in the [stg-8nn-scaffold-example](https://github.com/adolfogc/stg-8nn-scaffold-example) repository.

## Design Goals
- Use a modern embedded real-time framework, i.e., Quantum Leaps's QP™/C.
- Use Linux or macOS as the development platform.
- Use the [DroneCAN](https://dronecan.github.io/) protocol over CAN bus for communication.

## Main Software Components
- [QP™/C](https://www.state-machine.com) using the QK kernel.
- [ST's STM32Cube™](https://github.com/STMicroelectronics/STM32CubeF0), specifically, STM32F0's LL drivers and HAL.
- [Libcanard](https://github.com/dronecan/libcanard)

You should be acquainted with the QP™/C framework, DroneCAN and their concepts in order to use this project effectively.

## Building instructions

### Hardware
- BARTH® STG-8nn (mini-PLC)
- ST-Link v2 (In-Circuit Programmer)
- [MikroElektronika IrDA-To-PC Board (MIKROE-441)](https://www.mikroe.com/irda-to-pc-board)
- [Kvaser Leaf Light Rugged HS](https://www.kvaser.com/product/kvaser-leaf-light-rugged-hs)

### Getting the code

```bash
git clone https://github.com/adolfogc/stg-8nn-scaffold.git
cd stg-8nn-scaffold
git submodule init
git submodule update --init --recursive
```

### Getting the toolchain and the flashing utility

See `.devcontainer` directory.

### Compiling the project

```bash
mkdir build && cd build
cmake -DSTG_MODEL:STRING=850 -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -GNinja ..
cmake --build .
```

```bash
mkdir build && cd build
cmake -DSTG_MODEL:STRING=850 -DCMAKE_TOOLCHAIN_FILE=arm-clang-toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -GNinja ..
cmake --build .
```

4. Compilation database

The above commands use the `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` flag so that CMake generates a compilation database. You can use it to check how the source files will get compiled by inspecting the generated file `compile_commands.json`. Also, other tools like Cppcheck and Clang-Tidy use this file.

### Flashing the firmware
```bash
# Flash it using openocd:
openocd \
  -f ../openocd/stm32f0x-stlinkv2.cfg \
  -f ../openocd/stm32f0x-utils.cfg \
  -c "custom_flash firmware.bin" \
  -c shutdown

# Flash it using st-flash (currently not working for me):
st-flash write firmware.bin 0x0800000
```

## License
![AGPL-3](https://www.gnu.org/graphics/agplv3-with-text-162x68.png)

For more information see [COPYING](COPYING).

## References
A compilation of some useful references that were consulted when creating this project is available in the [doc/REFERENCES.md](doc/REFERENCES.md) file.
