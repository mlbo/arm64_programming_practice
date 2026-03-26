# 页表遍历 - 知识点讲解

## 1. ARM64 页表结构

ARM64 使用 4 级页表 (使用 4KB 页时):

```
Level 0: PGD (Page Global Directory)
Level 1: PUD (Page Upper Directory)
Level 2: PMD (Page Middle Directory)
Level 3: PTE (Page Table Entry)
```

### 地址分解

48 位虚拟地址分解:

```
[47:39] PGD Index  (9 bits, 512 entries)
[38:30] PUD Index  (9 bits, 512 entries)
[29:21] PMD Index  (9 bits, 512 entries)
[20:12] PTE Index  (9 bits, 512 entries)
[11:0]  Page Offset (12 bits, 4KB page)
```

## 2. 页表项格式

### Block 描述符 (PUD/PMD)

```
Bits [1:0]   = 01 (Block)
Bits [47:12] = 输出地址 (36 bits)
Bits [63:52] = 保留
Bits [11:2]  = 属性
```

### Table 描述符

```
Bits [1:0]   = 11 (Table)
Bits [47:12] = 下一级页表地址
Bits [63:52] = 保留
Bits [11:2]  = 属性
```

### Page 描述符 (PTE)

```
Bits [1:0]   = 11 (Page)
Bits [47:12] = 物理地址
Bits [63:52] = 保留
Bits [11:2]  = 属性
```

## 3. 页表项属性

| 位 | 名称 | 说明 |
|---|------|------|
| 0 | Valid | 有效位 |
| 1 | Table/Page | 类型位 |
| 5-2 | AttrIndx | 内存属性索引 |
| 6-7 | AP | 访问权限 |
| 10 | AF | 访问标志 |
| 11 | nG | 非 Global |
| 52 | Contiguous | 连续块 |
| 53 | PXN | 特权不可执行 |
| 54 | XN | 不可执行 |

## 4. 页表遍历算法

```cpp
u64* walk_pgtable(u64* pgd, u64 va) {
    u64 pgd_idx = (va >> 39) & 0x1FF;
    u64 pud_idx = (va >> 30) & 0x1FF;
    u64 pmd_idx = (va >> 21) & 0x1FF;
    u64 pte_idx = (va >> 12) & 0x1FF;

    // Level 0: PGD
    u64* pud = (u64*)(pgd[pgd_idx] & ~0xFFF);
    if (!pud) return nullptr;

    // Level 1: PUD
    u64* pmd = (u64*)(pud[pud_idx] & ~0xFFF);
    if (!pmd) return nullptr;

    // Level 2: PMD
    u64* pte = (u64*)(pmd[pmd_idx] & ~0xFFF);
    if (!pte) return nullptr;

    // Level 3: PTE
    return &pte[pte_idx];
}
```

## 5. 块映射

PUD 和 PMD 可以直接映射大块内存:

- PUD Block: 1GB
- PMD Block: 2MB

```cpp
// 1GB 块映射
pud[0] = pa | PTE_VALID | PTE_AF | MEM_ATTR_NORMAL;

// 2MB 块映射
pmd[0] = pa | PTE_VALID | PTE_AF | MEM_ATTR_NORMAL;
```

## 6. TTBR 寄存器

- TTBR0_EL1: 用户空间页表 (0x0000_0000_0000 - 0x0000_FFFF_FFFF)
- TTBR1_EL1: 内核空间页表 (0xFFFF_0000_0000 - 0xFFFF_FFFF_FFFF)

```cpp
// 设置 TTBR0
u64 ttbr0 = (u64)pgd | ASID;
asm volatile("msr ttbr0_el1, %0" :: "r"(ttbr0));
```

## 7. TCR_EL1 配置

```cpp
u64 tcr =
    (16UL << 0) |    // T0SZ = 16 (48-bit VA)
    (0UL << 6) |     // IRGN0
    (0UL << 8) |     // ORGN0
    (3UL << 14) |    // TG0 = 64KB (or 0 for 4KB)
    (1UL << 23) |    // EPD0 = 1 (enable TTBR0)
    (16UL << 16) |   // T1SZ
    (1UL << 23);     // EPD1
asm volatile("msr tcr_el1, %0" :: "r"(tcr));
```

## 8. TLB 管理

修改页表后需要刷新 TLB:

```cpp
// 刷新单个虚拟地址
void flush_tlb_va(u64 va) {
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

## 9. 地址转换示例

假设 VA = 0x123456789ABC:

```
PGD Index = (VA >> 39) & 0x1FF = 0
PUD Index = (VA >> 30) & 0x1FF = 0x91
PMD Index = (VA >> 21) & 0x1FF = 0x1A2
PTE Index = (VA >> 12) & 0x1FF = 0x345
Offset    = VA & 0xFFF = 0xABC
```

## 10. 调试技巧

### 检查页表项

```cpp
void dump_pte(u64* pte) {
    u64 val = *pte;
    printk("PTE: 0x%016lx\n", val);
    printk("  Valid: %s\n", (val & 1) ? "yes" : "no");
    printk("  PA: 0x%016lx\n", val & 0xFFFFFFFFF000);
    printk("  AttrIndx: %lu\n", (val >> 2) & 0x7);
    printk("  AP: %lu\n", (val >> 6) & 0x3);
}
```