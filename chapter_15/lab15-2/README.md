# Lab 15-2: 页表遍历

## 实验目标

实现页表遍历，理解虚拟地址到物理地址的转换过程。

## 核心知识点

- 页表结构
- 地址转换过程
- 缓存维护操作

## 构建与运行

```bash
make && make run
```

## 页表遍历过程

```
虚拟地址 (VA)
    │
    ├─ PGD Index (bits [47:39]) → PGD 表项
    │
    ├─ PUD Index (bits [38:30]) → PUD 表项
    │
    ├─ PMD Index (bits [29:21]) → PMD 表项
    │
    ├─ PTE Index (bits [20:12]) → PTE 表项
    │
    └─ Page Offset (bits [11:0])
            │
            ▼
       物理地址 (PA)
```

## 目录结构

```
lab15-2/
├── README.md
├── docs/
│   ├── knowledge.md
│   ├── code_explanation.md
│   └── exercises.md
├── scripts/
├── include/
│   └── arch/arm64/
│       └── cache.hpp
├── src/
│   ├── cacheinfo.c
│   ├── dump_pgtable.c
│   └── cache.S
└── Makefile
```

## 预期输出

```
parse cache info:
   L1: i-cache, cache line size 64
          i-cache: set 256 way 3 line_size 64 size 48KB
          d/u cache: set 256 way 2 line_size 64 size 32KB
   L2: unifed cache, cache line size 64
          d/u cache: set 1024 way 16 line_size 64 size 1024KB
Dump page table:
PGD[0]: 0x0000000000000000
PUD[0]: 0x0000000000000001
PMD[0]: 0x0000000000000001
PTE[0]: 0x0000000000000401
VA 0x00000000 -> PA 0x00000000
```