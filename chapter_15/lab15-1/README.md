# Lab 15: 缓存管理与页表遍历

本章包含两个实验:
- **lab15-1**: 缓存信息查询
- **lab15-2**: 页表遍历

## Lab 15-1: 缓存信息查询

### 实验目标

理解 ARM64 缓存层次结构，学会查询缓存信息。

### 核心知识点

- CTR_EL0: 缓存类型寄存器
- CLIDR_EL1: 缓存层次 ID 寄存器
- CCSIDR_EL1: 缓存大小 ID 寄存器
- 缓存行大小、组数、路数

### 构建与运行

```bash
cd lab15-1
make && make run
```

---

## Lab 15-2: 页表遍历

### 实验目标

实现页表遍历，理解虚拟地址到物理地址的转换过程。

### 核心知识点

- 页表结构
- 地址转换过程
- 缓存维护操作 (dc civac)

### 构建与运行

```bash
cd lab15-2
make && make run
```

---

## 目录结构

```
chapter_15/
├── lab15-1/
│   ├── README.md
│   ├── docs/
│   │   ├── knowledge.md
│   │   ├── code_explanation.md
│   │   └── exercises.md
│   ├── scripts/
│   ├── include/
│   │   └── arch/arm64/
│   │       └── cache.hpp
│   └── ...
└── lab15-2/
    ├── README.md
    ├── docs/
    ├── scripts/
    ├── include/
    │   └── arch/arm64/
    │       └── cache.hpp
    └── ...
```

## 预期输出

```
parse cache info:
   L1: i-cache, cache line size 64
          i-cache: set 256 way 3 line_size 64 size 48KB
          d/u cache: set 256 way 2 line_size 64 size 32KB
   L2: unifed cache, cache line size 64
          d/u cache: set 1024 way 16 line_size 64 size 1024KB
   IBC:2 LOUU:2 LoC:2 LoUIS:2
   Detected PIPT I-cache
```

## 参考资料

- ARM Architecture Reference Manual, D12: The Cache Maintenance Operations
- ARM Cortex-A Series Programmer's Guide, Chapter 12: Caches