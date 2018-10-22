# STG-8nn-Scaffold

<img align="right" width="80" height="100" src="stg-8nn-scaffold-logo.png">

A base project for programming the [BARTH® STG-8nn mini-PLCs](https://barth-elektronik.com/en/mini-plc.html).
It is a work-in-progress.

## Motivation
This base project aims to serve as a starting point for those programming in C the BARTH® STG-8nn mini-PLCs, using macOS or Linux as their development platform. Potential users acquainted with Quantum Leaps LLC's QP™ should be able to fork and modify it as needed.

## Design Goals
- Use a modern embedded real-time framework, i.e., Quantum Leaps LLC's QP™/C.
- Use Linux or macOS as the development platform.
- *[future work]* Use a CAN bus bootloader for on-the-field firmware updates.


## Main Software Components
- [QP™/C](https://www.state-machine.com) using the QK kernel.
- [libopencm3: An open source ARM® Cortex-M microcontroller library](https://github.com/libopencm3/libopencm3/wiki)

You should be acquainted with the QP™ framework and its concepts in order to use this base project effectively.

## Memory map
| Start address | Description| Size |
| -------------: |:-------------:|:--:|
| 0x08000000 | Application  | -|

## Building instructions

### Hardware
- BARTH® STG-8nn (mini-PLC)
- ST-Link v2 (In-Circuit Programmer)
- [Irdroid USB IrDA Transceiver](https://irdroid.eu/product/irdroid-usb-irda-transceiver)

### Getting the toolchain and the flashing utility
#### macOS
```bash
brew tap osx-cross/arm
brew install arm-gcc-bin stlink open-ocd cmake ninja
```
#### Linux
Pending.

### Compiling the project
```bash
mkdir build
cd build

export CC=arm-none-eabi-gcc
cmake -GNinja ${this_repos_dir}
ninja
```

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

Note: You can use the [STM32 ST-LINK utility](https://www.st.com/en/development-tools/stsw-link004.html) to update the ST-Link's firmware, but it only runs on Windows.

### Result

## License
![AGPL-3](https://www.gnu.org/graphics/agplv3-with-text-162x68.png)
For more information see [COPYING](COPYING).

## References

### STM32
- [STM32F091xC's datasheet](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=2ahUKEwiBzJGV0ZTeAhWKtlkKHYPmB78QFjAAegQICRAC&url=https%3A%2F%2Fwww.st.com%2Fresource%2Fen%2Fdatasheet%2Fdm00115237.pdf&usg=AOvVaw3K5QWoWjF2qagdtuoUGGnh)
- [STM32F091CC's web page](https://www.st.com/en/microcontrollers/stm32f091cc.html)
- [STM32F0xxx's Cortex-M0 programming manual](https://www.st.com/content/ccc/resource/technical/document/programming_manual/fc/90/c7/17/a1/44/43/89/DM00051352.pdf/files/DM00051352.pdf/jcr:content/translations/en.DM00051352.pdf)
- [STM32CubeF0's web page](https://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-mcu-packages/stm32cubef0.html)

### QP/C
- [*"Getting Started with QP/C"* guide](https://state-machine.com/doc/AN_Getting_Started_with_QPC.pdf)
- [*"QP Ports to ARM Cortex-M"*](http://www.state-machine.com/qpc/arm-cm.html)
- [*""*](http://www.state-machine.com/doc/AN_ARM-Cortex-M_Interrupt-Priorities.pdf)
- [*"Practical UML Statecharts in C/C++, 2nd Edition: Event-Driven Programming for Embedded Systems"*](https://www.state-machine.com/psicc2)

### Bootloaders
- [CVRA's CAN bootloader](https://github.com/cvra/can-bootloader)
- [STM32 Bootloader](https://github.com/akospasztor/stm32-bootloader)
- [*"Bootloader on STM32F0"* blog post](http://marcelojo.org/marcelojoeng/2015/09/bootloader-on-stm32f0.html)
- [*"Why every Cortex-M developer should consider using a bootloader"* article](http://blog.atollic.com/why-every-cortex-m-developer-should-consider-using-a-bootloader)

## LD Linker scripts
- [*"PC Memory - A tutorial on PC memory and hex arithmetic"*](https://savage.net.au/Ron/html/hex-ram-tutorial.html)
- [*"Using ld"*](http://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_mono/ld.html)

### CMake
- [*"An Introduction to Modern CMake"* book](https://cliutils.gitlab.io/modern-cmake)

### Other topics
- [*"Cutting Through the Confusion with ARM Cortex-M Interrupt Priorities"* blog post](https://embeddedgurus.com/state-space/2014/02/cutting-through-the-confusion-with-arm-cortex-m-interrupt-priorities)
- [osx-cross/homebrew-arm](https://github.com/osx-cross/homebrew-arm)
- [texane/stlink](https://github.com/texane/stlink)
- [libopencm3/libopencm3](https://github.com/libopencm3/libopencm3)
- [libopencm3/libopencm3-template](https://github.com/libopencm3/libopencm3-template)
- [libopencm3/libopencm3-examples](https://github.com/libopencm3/libopencm3-examples)
- [mossmann/hackrf](https://github.com/mossmann/hackrf)
