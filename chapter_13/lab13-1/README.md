# Lab 13-1: GICv2 中断控制器

## 实验目标

理解 ARM GICv2 中断控制器架构，实现基本的 GIC 驱动。

## 核心知识点

- GICv2 架构: Distributor + CPU Interface
- 中断类型: SGI, PPI, SPI
- 中断使能/禁用
- EOI (End of Interrupt)

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
lab13-1/
├── README.md
├── docs/
│   ├── knowledge.md
│   ├── code_explanation.md
│   └── exercises.md
├── scripts/
│   ├── build.sh
│   └── run.sh
├── include/
│   └── driver/
│       └── gicv2.hpp
├── src/
│   ├── driver/
│   │   └── gicv2.cpp
│   └── ...
├── Makefile
└── linker.ld
```

## GICv2 基地址

| 组件 | 基地址 |
|-----|-------|
| GIC Distributor | 0xFF841000 |
| GIC CPU Interface | 0xFF842000 |

## 中断 ID 分配

| 类型 | ID 范围 | 说明 |
|-----|--------|-----|
| SGI | 0-15 | 软件生成中断 (核间通信) |
| PPI | 16-31 | 私有外设中断 |
| SPI | 32+ | 共享外设中断 |

## 预期输出

```
gic_init: cpu_base:0xff842000, dist_base:0xff841000, gic_irqs:288
Core0 Timer interrupt received
Core0 Timer interrupt received
...
```

## 参考资料

- ARM Generic Interrupt Controller Architecture Specification Version 2.0
- BCM2711 ARM Peripherals Manual