# 中断与页表 - 知识点讲解

## 1. MMU 与中断的关系

当 MMU 启用后，所有地址访问都经过地址转换:

```
CPU 产生虚拟地址
       │
       ▼
   MMU 转换
       │
       ├─ 成功 → 物理地址 → 访问内存
       └─ 失败 → 触发异常 (Translation Fault)
```

### 中断处理中的地址转换

```
IRQ 发生
    │
    ▼
CPU 跳转到 VBAR_EL1 + 0x280
    │
    ▼
执行向量表代码 (虚拟地址)
    │
    ▼
kernel_entry 保存寄存器
    │
    ▼
调用中断处理函数 (虚拟地址)
    │
    ▼
访问 GIC 寄存器 (设备地址)
    │
    ▼
返回
```

## 2. 向量表映射

异常向量表必须满足:
1. 位于 4KB 对齐的地址
2. 所在页面已映射
3. 具有执行权限

```cpp
// 设置向量表
u64 vbar = (u64)&vectors;  // 虚拟地址
asm volatile("msr vbar_el1, %0" :: "r"(vbar));
```

## 3. 设备内存映射

GIC 等外设寄存器属于设备内存:

```
物理地址范围: 0xFF841000 - 0xFF843FFF (GIC)
映射属性: Device-nGnRnE
```

### 页表项属性

```cpp
// 设备内存页表项
constexpr u64 DEVICE_MEM_ATTR =
    (0ULL << 2) |   // AttrIdx = 0 (Device-nGnRnE)
    (1ULL << 10) |  // AF (Access Flag)
    (0ULL << 52);   // Contiguous
```

## 4. 页表配置流程

```cpp
void setup_mmu() {
    // 1. 创建页表
    create_page_tables();

    // 2. 设置 MAIR_EL1 (内存属性)
    u64 mair =
        (0x00ULL << 0) |   // Attr0: Device-nGnRnE
        (0xFFULL << 8) |   // Attr1: Normal Cached
        (0x44ULL << 16);   // Attr2: Normal Non-Cached
    asm volatile("msr mair_el1, %0" :: "r"(mair));

    // 3. 设置 TCR_EL1 (转换控制)
    u64 tcr =
        (0ULL << 37) |     // TBI0 = 0 (无顶部字节忽略)
        (0ULL << 30) |     // PS = 0 (32-bit 物理地址)
        (16ULL << 0) |     // T0SZ = 16 (48-bit VA)
        (0ULL << 6) |      // IRGN0 = 0
        (0ULL << 8) |      // ORGN0 = 0
        (1ULL << 23) |     // EPD0 = 1 (TTBR0 使用)
        (3ULL << 14);      // TG0 = 3 (64KB 页)
    asm volatile("msr tcr_el1, %0" :: "r"(tcr));

    // 4. 设置 TTBR0_EL1 (页表基地址)
    u64 ttbr0 = (u64)pgd | 1ULL;  // 页表基地址
    asm volatile("msr ttbr0_el1, %0" :: "r"(ttbr0));

    // 5. 使能 MMU
    u64 sctlr;
    asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr));
    sctlr |= (1 << 0);   // M = 1 (MMU 使能)
    sctlr |= (1 << 2);   // C = 1 (数据缓存使能)
    sctlr |= (1 << 12);  // I = 1 (指令缓存使能)
    asm volatile("msr sctlr_el1, %0" :: "r"(sctlr));

    // 6. 刷新 TLB
    asm volatile("isb");
    asm volatile("tlbi vmalle1");
    asm volatile("dsb ish");
    asm volatile("isb");
}
```

## 5. 中断安全

### 中断与原子操作

```cpp
// 禁用中断
void local_irq_disable() {
    asm volatile("msr daifset, #2");
}

// 使能中断
void local_irq_enable() {
    asm volatile("msr daifclr, #2");
}

// 保存并禁用中断
u64 local_irq_save() {
    u64 flags;
    asm volatile("mrs %0, daif" : "=r"(flags));
    asm volatile("msr daifset, #2");
    return flags;
}

// 恢复中断状态
void local_irq_restore(u64 flags) {
    asm volatile("msr daif, %0" :: "r"(flags));
}
```

## 6. 中断与缓存

### 缓存一致性

设备 DMA 访问内存时，需要考虑缓存一致性:
- CPU 写入数据后，需要 clean 缓存
- 设备写入数据后，CPU 需要 invalidate 缓存

```cpp
// clean 缓存到内存
void clean_dcache(void* addr, usize size) {
    u64 start = (u64)addr & ~0x3FULL;
    u64 end = (u64)addr + size;

    for (; start < end; start += 64) {
        asm volatile("dc civac, %0" :: "r"(start));
    }
    asm volatile("dsb ish");
}
```

## 7. 调试技巧

### 检查页表映射

```cpp
void dump_pgtable_entry(u64 va) {
    u64* pte = walk_pgtable(va);
    if (pte) {
        printk("VA 0x%lx -> PTE 0x%lx\n", va, *pte);
        if (*pte & 1) {
            u64 pa = (*pte & 0xFFFFFFFFF000ULL);
            printk("  PA: 0x%lx\n", pa);
        }
    }
}
```

### 检查 MMU 状态

```cpp
void check_mmu_status() {
    u64 sctlr;
    asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr));

    printk("MMU: %s\n", (sctlr & 1) ? "enabled" : "disabled");
    printk("I-Cache: %s\n", (sctlr & (1 << 12)) ? "enabled" : "disabled");
    printk("D-Cache: %s\n", (sctlr & (1 << 2)) ? "enabled" : "disabled");
}
```