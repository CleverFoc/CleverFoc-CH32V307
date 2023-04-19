# Firmware for CH32V307 FOC Controller

[中文](./Readme.md) | English   

[![Static Analysis](https://github.com/CleverFoc/CleverFoc-CH32V307/actions/workflows/cppcheck.yml/badge.svg)](https://github.com/CleverFoc/CleverFoc-CH32V307/actions/workflows/cppcheck.yml)
[![Build project](https://github.com/CleverFoc/CleverFoc-CH32V307/actions/workflows/build.yml/badge.svg)](https://github.com/CleverFoc/CleverFoc-CH32V307/actions/workflows/build.yml)

## 1.Introduction

This project is the firmware of [CH32V307 FOC Controller](https://github.com/LIUYIXIEYANG/CH32V307-Dual-brushless-motor-drive) . It can realize the FOC control of two motors, and support the speed and torque closed-loop control of the motor.

## 2.Project Structure

```
Root
├── core # cpu core related
├── ld # link script
├── LICENSE # open source license
├── package.json # project configuration
├── peripheral # peripheral library
├── Readme.md # project description
├── src # source code
├── startup # startup file
├── third_party # third-party library
└── xmake.lua # xmake configuration
```

## 3.How to develop

### 3.1 Prepare the environment

- Xmake -- project build tool
- VSCode -- code editor

The following tools are extracted from MounRiver Studio, and the mainline version of these tools is not supported by CH32V307.

- RISC-V GNU Toolchain -- RISC-V compiler
- OpenOCD -- debugger

The toolchain is located in `MounRiver_Studio\toolchain`, copy the `RISC-V Embedded GCC`, `OpenOCD` folders to the `tools` folder of this project.

If everything is configured properly, you can enter `xmake` in the terminal. If the following content appears, the environment configuration is successful.

```bash
$ xmake
[ 72%]: cache compiling.release src\driver\pwm_driver.c
[ 94%]: linking.release CleverFoc_CH32V307.elf
[100%]: build ok, spent 0.516s
```

### 3.2 Compile and download

#### 3.2.1 Compile

If you use VSCode, you can press `Ctrl + Shift + B` directly, select `xmake: build`, and then compile.
If you use the terminal, enter `xmake`, and then compile.

#### 3.2.2 Download

If you use VSCode, you can execute the pre-configured `Download` task, which will automatically compile and download to the development board through WCH-LINK.

Otherwise, please manually download through OpenOCD, refer to the following command.

```bash
$ ./tools/OpenOCD/bin/openocd.exe -f ./tools/OpenOCD/bin/wch-riscv.cfg -c init -c halt -c 'flash write_image erase ./build/cross/riscv/release/CleverFoc_CH32V307.hex' -c reset -c shutdown
```

## 4.Project Status

This project is still under development, and the following functions are being developed.

## 5.Contribution

If you have any questions, please submit an issue or contact me by email.

## 6.License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details


