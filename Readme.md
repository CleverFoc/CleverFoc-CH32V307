# 基于 CH32V307 的 FOC 控制器固件

## 1. 项目简介

本项目是 [CH32V307 的 FOC 控制器](https://github.com/LIUYIXIEYANG/CH32V307-Dual-brushless-motor-drive)的固件。可以实现两个电机的 FOC 控制，支持电机的速度、力矩控制。

## 2. 项目结构

```
├─libs # 依赖库
└─src  # 源代码
```

## 3. 如何开发

### 3.1 环境准备

- Xmake -- 项目构建工具
- VSCode -- 代码编辑器
- [riscv-none-embed-gcc-xpack](https://xpack.github.io/dev-tools/riscv-none-elf-gcc/) -- RISC-V 编译器
- Openocd -- 烧录、调试工具

### 3.2 设置 Xmake

你需要指定编译器的路径
```bash
xmake f -p riscv-none-embed-gcc -c -vD
```



