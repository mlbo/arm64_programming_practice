# 中断与页表 - 代码说明

## 文件结构

### 1. include/mm/pgtable.hpp

页表相关定义。

```cpp
#pragma once

#include <stdint.h>

namespace benos::mm {

// 页大小
constexpr usize PAGE_SHIFT = 12;
constexpr usize PAGE_SIZE  = 1ULL << PAGE_SHIFT;
constexpr usize PAGE_MASK  = ~(PAGE_SIZE - 1);

// 页表级别
constexpr usize PGTABLE_LEVELS = 3;  // 3 级页表

// 页表项属性
namespace pte_attr {
    constexpr u64 VALID    = (1ULL << 0);   // 有效
    constexpr u64 TABLE    = (1ULL << 1);   // 页表描述符
    constexpr u64 PAGE     = (1ULL << 1);   // 页描述符
    constexpr u64 UXN      = (1ULL << 54);  // 用户不可执行
    constexpr u64 PXN      = (1ULL << 53);  // 特权不可执行
    constexpr u64 AF       = (1ULL << 10);  // 访问标志
    constexpr u64 AP_RW    = (0ULL << 6);   // 读写
    constexpr u64 AP_RO    = (1ULL << 7);   // 只读
    constexpr u64 AP_USER  = (1ULL << 6);   // 用户可访问
}

// 内存属性索引
namespace mem_attr {
    constexpr u64 DEVICE   = (0ULL << 2);   // 设备内存
    constexpr u64 NORMAL   = (1ULL << 2);   // 普通内存
    constexpr u64 NC       = (2ULL << 2);   // 非缓存内存
}

/// 页表遍历
/// @param va 虚拟地址
/// @return 页表项指针，nullptr 表示未映射
auto walk_pgtable(u64 va) -> u64*;

/// 创建映射
/// @param va 虚拟地址
/// @param pa 物理地址
/// @param attr 页表项属性
/// @return 0 成功，负数失败
auto create_mapping(u64 va, u64 pa, u64 attr) -> int;

/// 初始化页表
void init_pgtable();

} // namespace benos::mm
```

### 2. src/mmu.c

MMU 初始化实现。

#### 页表创建

```c
#define PGD_SIZE  4096
#define PUD_SIZE  4096
#define PMD_SIZE  4096
#define PTE_SIZE  4096

static u64 pgd[PGD_SIZE / 8] __attribute__((aligned(4096)));
static u64 pud[PUD_SIZE / 8] __attribute__((aligned(4096)));
static u64 pmd[PMD_SIZE / 8] __attribute__((aligned(4096)));
static u64 pte[PTE_SIZE / 8] __attribute__((aligned(4096)));

void create_page_tables(void) {
    // 恒等映射: VA = PA
    // 0x00000000 - 0x3FFFFFFF (1GB)
    map_range(0x00000000, 0x00000000, 0x40000000,
              PTE_VALID | PTE_AF | MEM_ATTR_NORMAL);

    // 设备映射: VA = PA + 0x80000000
    // GIC: 0xFF841000 -> 0x7F841000
    map_range(0xFF800000, 0xFF800000, 0x100000,
              PTE_VALID | PTE_AF | MEM_ATTR_DEVICE);
}
```

#### MMU 使能

```c
void enable_mmu(void) {
    u64 val;

    // 设置 MAIR_EL1
    val = (0x00UL << 0) |   // Device-nGnRnE
          (0xFFUL << 8) |   // Normal Cached
          (0x44UL << 16);   // Normal Non-Cached
    asm volatile("msr mair_el1, %0" :: "r"(val));

    // 设置 TCR_EL1
    val = (0UL << 37) |     // TBI0
          (4UL << 32) |     // IPS = 4 (36-bit PA)
          (16UL << 0) |     // T0SZ = 16
          (0UL << 6) |      // IRGN0
          (0UL << 8) |      // ORGN0
          (1UL << 23) |     // EPD0
          (3UL << 14);      // TG0 = 64KB
    asm volatile("msr tcr_el1, %0" :: "r"(val));

    // 设置 TTBR0_EL1
    asm volatile("msr ttbr0_el1, %0" :: "r"((u64)pgd));

    // 使能 MMU
    asm volatile("mrs %0, sctlr_el1" : "=r"(val));
    val |= (1 << 0);   // M = 1
    val |= (1 << 2);   // C = 1
    val |= (1 << 12);  // I = 1
    asm volatile("msr sctlr_el1, %0" :: "r"(val));

    // 刷新
    asm volatile("isb");
}
```

### 3. 中断处理中的地址

```cpp
// 向量表地址
extern char vectors[];

void setup_vector_table() {
    u64 vbar = (u64)&vectors;
    asm volatile("msr vbar_el1, %0" :: "r"(vbar));
}
```

### 4. 页表刷新

```cpp
// 刷新单个虚拟地址的 TLB
void flush_tlb(u64 va) {
    asm volatile("tlbi vaae1is, %0" :: "r"(va >> 12));
    asm volatile("dsb ish");
    asm volatile("isb");
}

// 刷新整个 TLB
void flush_tlb_all() {
    asm volatile("tlbi vmalle1is");
    asm volatile("dsb ish");
    asm volatile("isb");
}
```

### 5. 内存属性

| AttrIdx | MAIR | 说明 |
|---------|------|------|
| 0 | 0x00 | Device-nGnRnE |
| 1 | 0xFF | Normal Cached (RW-Allocate) |
| 2 | 0x44 | Normal Non-Cached |

```cpp
// 设置 MAIR_EL1
constexpr u64 MAIR_EL1_INIT =
    (0x00ULL << 0) |   // Attr0: Device-nGnRnE
    (0xFFULL << 8) |   // Attr1: Normal Cached
    (0x44ULL << 16);   // Attr2: Normal Non-Cached
```