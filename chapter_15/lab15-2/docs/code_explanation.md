# 页表遍历 - 代码说明

## 文件结构

### 1. src/dump_pgtable.c

```c
#include "io.h"
#include "type.h"

#define PGD_INDEX(va)   (((va) >> 39) & 0x1FF)
#define PUD_INDEX(va)   (((va) >> 30) & 0x1FF)
#define PMD_INDEX(va)   (((va) >> 21) & 0x1FF)
#define PTE_INDEX(va)   (((va) >> 12) & 0x1FF)
#define PAGE_OFFSET(va) ((va) & 0xFFF)

void dump_pgtable(unsigned long *pgd, unsigned long va)
{
    unsigned long *pud, *pmd, *pte;
    unsigned long pa;

    printk("Dump page table:\n");

    // PGD
    printk("PGD[%lu]: 0x%016lx\n", PGD_INDEX(va), pgd[PGD_INDEX(va)]);
    pud = (unsigned long *)(pgd[PGD_INDEX(va)] & ~0xFFF);
    if (!pud || !(pgd[PGD_INDEX(va)] & 1)) {
        printk("PGD not mapped\n");
        return;
    }

    // PUD
    printk("PUD[%lu]: 0x%016lx\n", PUD_INDEX(va), pud[PUD_INDEX(va)]);
    pmd = (unsigned long *)(pud[PUD_INDEX(va)] & ~0xFFF);
    if (!pmd || !(pud[PUD_INDEX(va)] & 1)) {
        printk("PUD not mapped\n");
        return;
    }

    // PMD
    printk("PMD[%lu]: 0x%016lx\n", PMD_INDEX(va), pmd[PMD_INDEX(va)]);
    pte = (unsigned long *)(pmd[PMD_INDEX(va)] & ~0xFFF);
    if (!pte || !(pmd[PMD_INDEX(va)] & 1)) {
        printk("PMD not mapped\n");
        return;
    }

    // PTE
    printk("PTE[%lu]: 0x%016lx\n", PTE_INDEX(va), pte[PTE_INDEX(va)]);
    pa = pte[PTE_INDEX(va)];
    if (pa & 1) {
        pa &= ~0xFFF;
        printk("VA 0x%016lx -> PA 0x%016lx\n", va, pa | PAGE_OFFSET(va));
    } else {
        printk("Not mapped\n");
    }
}
```

### 2. src/cache.S

缓存维护汇编函数。

```asm
/*
 * cache_clean_by_va: 按 VA clean 缓存
 * x0: 虚拟地址
 */
.global cache_clean_by_va
cache_clean_by_va:
    dc cvac, x0
    dsb ish
    isb
    ret

/*
 * cache_clean_inv_by_va: 按 VA clean and invalidate
 * x0: 虚拟地址
 */
.global cache_clean_inv_by_va
cache_clean_inv_by_va:
    dc civac, x0
    dsb ish
    isb
    ret

/*
 * cache_inv_by_va: 按 VA invalidate
 * x0: 虚拟地址
 */
.global cache_inv_by_va
cache_inv_by_va:
    dc ivac, x0
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

/*
 * cache_flush_range: 刷新一段内存的缓存
 * x0: 起始地址
 * x1: 结束地址
 */
.global cache_flush_range
cache_flush_range:
    sub     x1, x1, x0
    add     x1, x1, x0
    mrs     x2, ctr_el0
    lsr     x2, x2, #16
    and     x2, x2, #0xf
    mov     x3, #4
    lsl     x3, x3, x2      // x3 = cache line size

    and     x0, x0, ~(x3 - 1)  // 对齐到缓存行

1:
    dc      civac, x0
    add     x0, x0, x3
    cmp     x0, x1
    b.cc    1b

    dsb     ish
    isb
    ret
```

### 3. 地址转换

```c
// 将虚拟地址转换为物理地址
unsigned long va_to_pa(unsigned long *pgd, unsigned long va)
{
    unsigned long *pud, *pmd, *pte;

    pud = (unsigned long *)(pgd[PGD_INDEX(va)] & ~0xFFF);
    if (!pud) return 0;

    pmd = (unsigned long *)(pud[PUD_INDEX(va)] & ~0xFFF);
    if (!pmd) return 0;

    pte = (unsigned long *)(pmd[PMD_INDEX(va)] & ~0xFFF);
    if (!pte) return 0;

    return (pte[PTE_INDEX(va)] & ~0xFFF) | PAGE_OFFSET(va);
}
```

### 4. 页表属性检查

```c
int check_pte_attrs(unsigned long pte)
{
    if (!(pte & 1)) {
        printk("Not valid\n");
        return -1;
    }

    printk("AttrIndx: %lu\n", (pte >> 2) & 0x7);
    printk("AP: %lu (", (pte >> 6) & 0x3);

    u64 ap = (pte >> 6) & 0x3;
    switch (ap) {
    case 0: printk("RW, EL1 only)\n"); break;
    case 1: printk("RW, EL0/EL1)\n"); break;
    case 2: printk("RO, EL1 only)\n"); break;
    case 3: printk("RO, EL0/EL1)\n"); break;
    }

    printk("AF: %s\n", (pte & (1 << 10)) ? "set" : "not set");
    printk("nG: %s\n", (pte & (1 << 11)) ? "not global" : "global");

    return 0;
}
```

### 5. 内存屏障

缓存操作后需要使用内存屏障:

```c
// dsb (Data Synchronization Barrier)
// 确保之前的内存访问完成
asm volatile("dsb ish");  // Inner Shareable

// isb (Instruction Synchronization Barrier)
// 确保流水线中的指令被刷新
asm volatile("isb");
```