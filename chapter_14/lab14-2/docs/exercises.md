# 中断与页表 - 练习题

## 练习 1: 页表遍历

**任务：** 实现函数 `walk_pgtable(u64 va)` 返回页表项指针:

```cpp
u64* walk_pgtable(u64 va) {
    u64 pgd_index = (va >> 30) & 0x1FF;
    u64 pud_index = (va >> 21) & 0x1FF;
    u64 pmd_index = (va >> 12) & 0x1FF;
    // TODO: 完成实现
}
```

---

## 练习 2: 映射创建

**任务：** 实现 `create_mapping(u64 va, u64 pa, u64 attr)`:

```cpp
int create_mapping(u64 va, u64 pa, u64 attr) {
    u64* pte = walk_pgtable(va);
    if (!pte) return -1;
    *pte = pa | attr;
    return 0;
}
```

**问题：** 需要刷新 TLB 吗？

---

## 练习 3: 恒等映射

**问题：** 为什么在启用 MMU 前需要建立恒等映射 (VA = PA)？

**提示：** 考虑 `msr sctlr_el1` 指令执行后，PC 指向的地址。

---

## 练习 4: 设备内存属性

**问题：** 解释以下页表项属性的含义:

```cpp
u64 pte = PA |
    PTE_VALID |
    PTE_AF |
    (0ULL << 2) |  // AttrIdx = 0
    PTE_UXN |
    PTE_PXN;
```

**提示：** AttrIdx = 0 对应什么内存类型？

---

## 练习 5: 页表大小计算

**问题：** 对于 48 位虚拟地址空间，计算:
1. 每级页表的表项数
2. 每级页表的大小 (使用 4KB 页)

---

## 练习 6: TLB 刷新

**问题：** 为什么修改页表后需要刷新 TLB？

```cpp
void update_mapping(u64 va, u64 new_pa) {
    u64* pte = walk_pgtable(va);
    *pte = new_pa | PTE_VALID | PTE_AF;
    // TODO: 需要 TLB 刷新吗？
}
```

---

## 练习 7: 中断与原子性

**问题：** 以下代码是否线程安全？如何修改？

```cpp
int counter = 0;

void increment_counter() {
    counter++;
}
```

**提示：** 考虑中断可能在任意时刻发生。

---

## 练习 8: 地址转换调试

**任务：** 实现函数打印虚拟地址的页表信息:

```cpp
void dump_va_info(u64 va) {
    printk("VA: 0x%016lx\n", va);
    // 打印各级页表索引
    // 打印最终物理地址
    // 打印内存属性
}
```

---

## 练习 9: 缓存一致性

**问题：** 设备 DMA 读取内存前，为什么需要 clean 缓存？

```cpp
void prepare_for_dma_read(void* buf, usize size) {
    // TODO: 实现缓存 clean
}
```

---

## 练习 10: MMU 错误处理

**问题：** 如果发生 Translation Fault，如何调试？

1. 检查 `ESR_EL1` 获取错误类型
2. 检查 `FAR_EL1` 获取错误地址
3. 检查页表是否正确映射

```cpp
void handle_translation_fault(u64 far, u32 esr) {
    printk("Translation Fault at 0x%lx, ESR=0x%08x\n", far, esr);
    // TODO: 分析原因并打印调试信息
}
```