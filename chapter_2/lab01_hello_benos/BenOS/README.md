# Lab 01: Hello BenOS

## 实验目标

建立第一个 ARM64 裸机程序，理解启动流程和 UART 输出。

## 硬件平台

- Raspberry Pi 4B (默认)
- Raspberry Pi 3B (可选，使用 `make board=rpi3`)

## 编译与运行

```bash
# 编译
make

# 运行 (QEMU)
make run

# 调试
make debug
```

## 预期输出

```
Welcome BenOS!
```

然后程序进入回显模式，在终端输入的字符会被原样返回。

## 文件结构

```
BenOS/
├── README.md              # 本文件
├── docs/
│   ├── knowledge.md       # 知识点讲解
│   ├── code_explanation.md # 代码说明
│   └── exercises.md       # 练习题
├── scripts/
│   ├── build.sh           # 编译脚本
│   └── run.sh             # 运行脚本
├── include/
│   ├── types.hpp          # 类型定义
│   ├── uart.hpp           # UART 驱动类
│   ├── io.hpp             # MMIO 操作
│   └── mm.hpp             # 内存常量
├── src/
│   ├── boot.S             # 启动汇编
│   ├── mm.S               # 内存操作
│   ├── pl_uart.cpp        # UART 实现
│   └── kernel.cpp         # 内核入口
├── Makefile
└── linker.ld
```

## 学习重点

1. **启动流程**: 从 `_start` 到 `kernel_main` 的执行过程
2. **多核控制**: 只有 CPU0 执行主程序，其他核心挂起
3. **UART 初始化**: GPIO 配置和波特率设置
4. **MMIO 操作**: 内存映射 I/O 的读写方法