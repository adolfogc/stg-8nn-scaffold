## Source
Files in this directory were taken from the *"STM32Cube MCU Package for STM32F0 series (HAL, Low-Layer APIs and CMSIS (CORE, DSP, RTOS), USB, File system, RTOS, Touch Sensing - coming with examples running on ST boards: STM32 Nucleo, Discovery kits and Evaluation boards)"*, part number ** STM32CubeF0**, version **1.9.0**, available [here](https://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-mcu-packages/stm32cubef0.html).

## Directory structure
The files were rearranged in a different directory structure to the one provided in ST's package:
- `cmsis` contains the CMSIS-compliant interface to the STM32F091xC
MCU
- `hal` contains part of the STM32Cube's HAL (Hardware Abstraction Layer)
- `gnu` contains the startup code for use with the GNU GCC toolchain

The STM32Cube's LL (Low Layer APIs) are not used in this project.
