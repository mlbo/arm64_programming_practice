# 页表遍历 - 练习题

## 练习 1: 地址索引计算

**问题：** 计算虚拟地址 0x123456789ABC 的各级索引:

```
PGD Index = ?
PUD Index = ?
PMD Index = ?
PTE Index = ?
Offset = ?
```

---

## 练习 2: 页表项解析

**问题：** 给定页表项值 0x0000000080000751，解析:

1. 是否有效？
2. 物理地址是什么？
3. AttrIndx 是多少？
4. AP (访问权限) 是什么？

---

## 练习 3: 实现地址转换

**任务：** 实现函数 `va_to_pa`:

```cpp
u64 va_to_pa(u64* pgd, u64 va) {
    // TODO: 实现
}
```

---

## 练习 4: 块映射

**问题：** PUD Block 和 PMD Block 分别映射多大的内存？

**任务：** 创建一个 2MB 的块映射:

```cpp
void map_2mb_block(u64* pmd, u64 va, u64 pa) {
    u64 pmd_idx = (va >> 21) & 0x1FF;
    pmd[pmd_idx] = pa | PTE_VALID | PTE_AF | MEM_ATTR_NORMAL;
}
```

---

## 练习 5: TLB 刷新

**问题：** 什么时候需要刷新 TLB？

**任务：** 实现 TLB 刷新函数:

```cpp
void flush_tlb_va(u64 va) {
    // TODO: 使用 tlbi 指令
}
```

---

## 练习 6: 权限检查

**任务：** 实现函数检查页表项的权限:

```cpp
bool check_read_permission(u64 pte);
bool check_write_permission(u64 pte);
bool check_execute_permission(u64 pte, bool is_el0);
```

---

## 练习 7: 大页支持

**问题：** ARM64 支持哪些大小的页？

- 4KB 页
- 64KB 页
- 2MB 大页
- 1GB 大页

**任务：** 解释 TCR_EL1 的 TG0 字段。

---

## 练习 8: 页表对齐

**问题：** 为什么页表基地址必须 4KB 对齐？

**提示：** 考虑 TTBR 寄存器的格式。

---

## 练习 9: ASID

**问题：** ASID (Address Space ID) 的作用是什么？

**任务：** 解释以下代码:

```cpp
u64 ttbr0 = (u64)pgd | (asid << 48);
asm volatile("msr ttbr0_el1, %0" :: "r"(ttbr0));
```

---

## 练习 10: 调试 Translation Fault

**问题：** 当发生 Translation Fault 时，如何调试？

**步骤：**
1. 读取 FAR_EL1 获取错误地址
2. 读取 ESR_EL1 获取错误原因
3. 遍历页表检查映射

```cpp
void debug_translation_fault() {
    u64 far;
    u32 esr;
    asm volatile("mrs %0, far_el1" : "=r"(far));
    asm volatile("mrs %0, esr_el1" : "=r"(esr));

    printk("Translation fault at 0x%lx\n", far);
    printk("ESR: 0x%08x\n", esr);

    // TODO: 遍历页表
}
```