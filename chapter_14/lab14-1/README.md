# Lab 14: 中断与定时器管理

本章包含两个实验:
- **lab14-1**: 定时器中断
- **lab14-2**: 中断与页表 (综合)

## Lab 14-1: 定时器中断

### 实验目标

理解 ARM Generic Timer，实现定时器中断处理。

### 核心知识点

- ARM Generic Timer 架构
- CNTP_CTL_EL0, CNTP_TVAL_EL0 寄存器
- 定时器中断配置

### 构建与运行

```bash
cd lab14-1
make && make run
```

---

## Lab 14-2: 中断与页表

### 实验目标

综合运用 GIC + Timer + MMU，实现完整的中断处理流程。

### 核心知识点

- 中断处理完整流程
- 页表与中断的关系
- MMU 配置

### 构建与运行

```bash
cd lab14-2
make && make run
```

---

## 目录结构

```
chapter_14/
├── lab14-1/
│   ├── README.md
│   ├── docs/
│   │   ├── knowledge.md
│   │   ├── code_explanation.md
│   │   └── exercises.md
│   ├── scripts/
│   ├── include/
│   │   └── driver/
│   │       ├── timer.hpp
│   │       └── irq_manager.hpp
│   ├── src/
│   └── Makefile
└── lab14-2/
    ├── ...
```

## 预期输出

```
gic_init: cpu_base:0xff842000, dist_base:0xff841000, gic_irqs:288
Core0 Timer interrupt received
Core0 Timer interrupt received
...
```

## 参考资料

- ARM Architecture Reference Manual, D13: The Generic Timer
- ARM Cortex-A Series Programmer's Guide, Chapter 11: Timer