# Debugging

We use VS Code with the Cortex-Debug plugin installed for debugging code running in the uC.

## Assumptions
- We're using an ST-Link v2 (In-Circuit Programmer).
- We're running on Linux (K)Ubuntu 19.04.
- We're using the official ARM GNU Toolchain (2918-q4-major) installed in the /opt directory (/opt/gcc-arm-none-eabi-8-2018-q4-major/bin).

## Installing VS Code

Download the .deb package from [VS Code's web site](https://code.visualstudio.com/Download) and install using `dpkg`.

## Installing the Cortex-Debug plugin.
In VS Code search for `cortex-debug` to install [Cortex-Debug](https://github.com/Marus/cortex-debug). 

## Configure a debugger for our Cortex-M uC.

Use the following configuration:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "cortex-debug",
            "request": "launch",
            "name": "STM32F091x Debug (OpenOCD)",
            "servertype": "openocd",
            "armToolchainPath": "/opt/gcc-arm-none-eabi-8-2018-q4-major/bin",
            "openocdPath": "/usr/bin",
            "cwd": "${workspaceRoot}",
            "executable": "./build/firmware.elf",
            "runToMain": true,
            "device": "STM32F091x",
            "svdFile": "openocd/STM32F091x.svd",
            "configFiles": [
                "openocd/stm32f0x-stlinkv2.cfg"
            ]
        }
    ]
}
```

Note: this configuration is stored in `.vscode/launch.json`.



