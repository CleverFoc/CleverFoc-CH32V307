# 基于 CH32V307 的 FOC 控制器固件

中文 | [English](./Readme.en_Us.md)
## 1. 项目简介

本项目是 [CH32V307 的 FOC 控制器](https://github.com/LIUYIXIEYANG/CH32V307-Dual-brushless-motor-drive) 的固件。可以实现两个电机的 FOC 控制，支持电机的速度、力矩闭环控制。

## 2. 项目结构

```
Root
├── core # cpu 内核相关
├── ld # 链接脚本
├── LICENSE # 开源协议
├── package.json # 项目配置
├── peripheral # 外设库
├── Readme.md # 项目说明
├── src # 源码
├── startup # 启动文件
├── third_party # 第三方库
└── xmake.lua # xmake 配置
```

## 3. 如何开发

### 3.1 环境准备

- Xmake -- 项目构建工具
- VSCode -- 代码编辑器

以下工具请从 MounRiver Studio 中提取，CH32V307 不支持这些工具的主线版本。

- RISC-V GNU Toolchain -- RISC-V 编译器
- OpenOCD -- 调试器

工具链位于 `MounRiver_Studio\toolchain`,将 `RISC-V Embedded GCC`, `OpenOCD` 文件夹复制到本项目的 `tools` 文件夹中。

如果一切皆配置妥当，可以在终端中输入 `xmake`，如果出现以下内容，说明环境配置成功。

```bash
$ xmake
[ 72%]: cache compiling.release src\driver\pwm_driver.c
[ 94%]: linking.release CleverFoc_CH32V307.elf
[100%]: build ok, spent 0.516s
```

### 3.2 编译和烧录

#### 3.2.1 编译

如果你使用 VSCode，可以直接按下 `Ctrl + Shift + B`，选择 `xmake: build`，即可编译。    
如果你使用终端，输入 `xmake`，即可编译。

#### 3.2.2 烧录

如果你使用 VSCode，可以执行预先配置好的 `Download` 任务，将自动执行编译，并通过 WCH-LINK 烧录到开发板。

否则请手动通过 OpenOCD 烧录，参考以下命令。

```bash
$ ./tools/OpenOCD/bin/openocd.exe -f ./tools/OpenOCD/bin/wch-riscv.cfg -c init -c halt -c 'flash write_image erase ./build/cross/riscv/release/CleverFoc_CH32V307.hex' -c reset -c shutdown
```

## 4. 项目状态

目前处于早期开发阶段，还未实现完整的功能。项目中目前多为测试代码，可能会有大量的 BUG，甚至会导致硬件损坏。

## 5. 贡献

如果你有任何问题，欢迎提出 [issue](https://github.com/CleverFoc/CleverFoc-CH32V307/issues) 或者 [PR](https://github.com/CleverFoc/CleverFoc-CH32V307/pulls)。

## 6. 许可

本项目采用 [MIT](https://opensource.org/license/mit/) 开源协议。    

