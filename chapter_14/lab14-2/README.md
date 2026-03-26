# Lab 14-2: 中断与页表 (综合)

## 实验目标

综合运用 GIC + Timer + MMU，实现完整的中断处理流程，并理解页表与中断的关系。

## 核心知识点

- 完整中断处理流程
- 页表配置
- 中断向量表与 MMU

## 构建与运行

```bash
make && make run
```

## 实验内容

本实验在 lab14-1 基础上增加:
1. MMU 页表配置
2. 虚拟地址映射
3. 中断处理中的地址转换

## 页表与中断的关系

### 1. 向量表地址

异常向量表由 `VBAR_EL1` 寄存器指定。如果启用 MMU:
- 向量表地址可以是虚拟地址
- 需要确保向量表所在页面已映射

### 2. 中断处理中的地址

中断处理中访问的地址:
- `VBAR_EL1` 指向的向量表
- 中断处理函数代码
- 中断处理中的数据访问
- GIC 寄存器地址

所有这些地址必须正确映射。

### 3. 设备内存映射

GIC 寄存器属于设备内存，映射属性:
- Device-nGnRnE 或 Device-nGnRE
- 不可缓存

## 目录结构

```
lab14-2/
├── README.md
├── docs/
│   ├── knowledge.md
│   ├── code_explanation.md
│   └── exercises.md
├── scripts/
├── include/
│   ├── driver/
│   │   ├── gicv2.hpp
│   │   └── timer.hpp
│   └── mm/
│       └── pgtable.hpp
├── src/
│   └── mmu.c
└── Makefile
```

## 预期输出

```
MMU enabled
gic_init: cpu_base:0xff842000, dist_base:0xff841000, gic_irqs:288
Core0 Timer interrupt received
Core0 Timer interrupt received
...
```