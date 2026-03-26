# Lab 12-1: 中断处理与栈帧

## 实验目标

理解 ARM64 异常处理机制，掌握中断栈帧 (pt_regs) 的保存与恢复。

## 核心知识点

- `kernel_entry` / `kernel_exit` 宏
- `pt_regs` 结构体
- `SPSR_EL1`, `ELR_EL1` 寄存器
- 异常向量表

## 构建与运行

```bash
# 编译
make

# 运行 (QEMU)
make run

# 调试
make debug
```

## 目录结构

```
lab12-1/
├── README.md              # 本文件
├── docs/
│   ├── knowledge.md       # 知识点讲解
│   ├── code_explanation.md # 代码说明
│   └── exercises.md       # 练习题
├── scripts/
│   ├── build.sh           # 编译脚本
│   └── run.sh             # 运行脚本
├── include/
│   ├── arch/arm64/
│   │   └── pt_regs.hpp    # pt_regs 结构体
│   └── ...
├── src/
│   ├── entry.S            # 异常向量表
│   ├── kernel.cpp         # 内核主函数
│   └── ...
├── Makefile
└── linker.ld
```

## 预期输出

```
Welcome BenOS!
printk init done
...
Core0 Timer interrupt received
Core0 Timer interrupt received
...
```

## 参考资料

- ARM Architecture Reference Manual ARMv8, D1.10: Exception entry
- ARM Cortex-A Series Programmer's Guide, Chapter 10: Exception Handling