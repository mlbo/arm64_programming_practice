# 缓存管理 - 代码说明

## 文件结构

### 1. include/arch/arm64/cache.hpp

```cpp
#pragma once

#include <stdint.h>

namespace benos::arch::arm64 {

/// 缓存信息结构
struct CacheInfo {
    u32 line_size;   ///< 缓存行大小 (字节)
    u32 sets;        ///< 组数
    u32 ways;        ///< 路数
    u32 level;       ///< 缓存级别 (1-7)
    u32 size_kb;     ///< 缓存大小 (KB)
    bool is_instruction; ///< 是否为指令缓存
};

/// 缓存类型
enum class CacheType : u32 {
    None     = 0,  ///< 无缓存
    Instr    = 1,  ///< 仅指令缓存
    Data     = 2,  ///< 仅数据缓存
    Separate = 3,  ///< 分离缓存 (I$ + D$)
    Unified  = 4,  ///< 统一缓存
};

/// 缓存维护类
class CacheMaintainer {
public:
    /// 获取缓存信息
    static auto get_cache_info(u32 level, bool is_instruction) -> CacheInfo;

    /// 按 VA clean 缓存
    static void clean_by_va(u64 va);

    /// 按 VA invalidate 缓存
    static void invalidate_by_va(u64 va);

    /// 按 VA clean and invalidate
    static void clean_and_invalidate_by_va(u64 va);

    /// 按组路 clean 整个缓存
    static void clean_all(u32 level);

    /// 按组路 invalidate 整个缓存
    static void invalidate_all(u32 level);

    /// 刷新指令缓存
    static void invalidate_icache_all();

    /// 获取缓存行大小
    static auto get_dcache_line_size() -> u32;
    static auto get_icache_line_size() -> u32;

private:
    static auto read_ccsidr(u32 level, bool is_instruction) -> u64;
};

/// 页表遍历器
class PageTableWalker {
public:
    /// 遍历页表，返回物理地址
    static auto walk(u64 va) -> u64;

    /// 获取物理地址 (别名)
    static auto get_pa(u64 va) -> u64 {
        return walk(va);
    }
};

} // namespace benos::arch::arm64
```

### 2. src/cacheinfo.c

#### 读取缓存信息

```c
static void get_cache_set_way(unsigned int level, unsigned int ind)
{
    unsigned long val;
    unsigned int line_size, set, way;

    // 1. 写 CSSELR_EL1 选择缓存级别
    unsigned int csselr = (level - 1) << 1 | ind;
    asm volatile("msr csselr_el1, %0" :: "r"(csselr));
    isb();

    // 2. 读 CCSIDR_EL1 获取缓存参数
    asm volatile("mrs %0, ccsidr_el1" : "=r"(val));

    set = ((val >> 13) & 0x1FFF) + 1;
    way = ((val >> 3) & 0x3FF) + 1;
    line_size = 1 << ((val & 0x7) + 4);

    printk("          %s: set %u way %u line_size %u size %uKB\n",
           ind ? "i-cache" : "d/u cache", set, way, line_size,
           (line_size * way * set) / 1024);
}
```

#### 解析缓存层次

```c
int init_cache_info(void)
{
    int level;
    unsigned long ctype;

    printk("parse cache info:\n");

    for (level = 1; level <= 7; level++) {
        ctype = get_cache_type(level);
        if (ctype == CACHE_TYPE_NOCACHE) {
            level--;
            break;
        }

        printk("   L%u: %s, cache line size %u\n",
               level, cache_type_string[ctype], cache_line_size());

        if (ctype == CACHE_TYPE_SEPARATE) {
            get_cache_set_way(level, 1);  // I-Cache
            get_cache_set_way(level, 0);  // D-Cache
        } else {
            get_cache_set_way(level, 0);  // Unified
        }
    }

    // 打印 CLIDR 摘要信息
    unsigned long clidr;
    asm volatile("mrs %0, clidr_el1" : "=r"(clidr));
    printk("   IBC:%u LOUU:%u LoC:%u LoUIS:%u\n",
           (clidr >> 30) & 0x7,
           (clidr >> 27) & 0x7,
           (clidr >> 24) & 0x7,
           (clidr >> 21) & 0x7);

    return level;
}
```

### 3. src/cache.S

缓存维护汇编实现。

```asm
/*
 * clean_by_va: 按 VA clean 缓存
 * x0: 虚拟地址
 */
.global cache_clean_by_va
cache_clean_by_va:
    dc cvac, x0
    dsb ish
    isb
    ret

/*
 * clean_and_invalidate_by_va: 按 VA clean and invalidate
 * x0: 虚拟地址
 */
.global cache_clean_inv_by_va
cache_clean_inv_by_va:
    dc civac, x0
    dsb ish
    isb
    ret

/*
 * invalidate_icache_all: 刷新整个指令缓存
 */
.global invalidate_icache_all
invalidate_icache_all:
    ic iallu
    dsb ish
    isb
    ret
```

### 4. 页表遍历

```c
// dump_pgtable.c
void dump_pgtable(unsigned long *pgd, unsigned long va)
{
    unsigned long *pud, *pmd, *pte;
    unsigned long pa;

    // PGD
    pud = (unsigned long *)(pgd[PGD_INDEX(va)] & ~0xFFF);
    if (!pud) {
        printk("PGD not mapped\n");
        return;
    }

    // PUD
    pmd = (unsigned long *)(pud[PUD_INDEX(va)] & ~0xFFF);
    if (!pmd) {
        printk("PUD not mapped\n");
        return;
    }

    // PMD
    pte = (unsigned long *)(pmd[PMD_INDEX(va)] & ~0xFFF);
    if (!pte) {
        printk("PMD not mapped\n");
        return;
    }

    // PTE
    pa = pte[PTE_INDEX(va)];
    if (pa & 1) {
        printk("VA 0x%lx -> PA 0x%lx\n", va, pa & ~0xFFF);
    } else {
        printk("Not mapped\n");
    }
}
```

### 5. 缓存行对齐

```cpp
// 获取缓存行对齐的地址
u64 cache_line_align(u64 addr) {
    u32 line_size = CacheMaintainer::get_dcache_line_size();
    return addr & ~(line_size - 1);
}

// 刷新一个内存范围
void clean_range(void* addr, usize size) {
    u64 start = (u64)addr;
    u64 end = start + size;
    u32 line_size = CacheMaintainer::get_dcache_line_size();

    start &= ~(line_size - 1);
    for (; start < end; start += line_size) {
        CacheMaintainer::clean_by_va(start);
    }
}
```