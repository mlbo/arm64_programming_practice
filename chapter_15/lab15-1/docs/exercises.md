# 缓存管理 - 练习题

## 练习 1: 缓存行大小计算

**问题：** CTR_EL0 寄存器的 DminLine 字段存储的是 log2 值减 4。解释以下代码:

```cpp
u32 line_size = 4 << ((ctr >> 16) & 0xF);
```

**测试：** 如果 DminLine = 4，缓存行大小是多少？

---

## 练习 2: 缓存大小计算

**问题：** 从 CCSIDR_EL1 读取到:
- NumSets = 255
- Associativity = 3
- LineSize = 3

计算缓存总大小。

---

## 练习 3: CLIDR 解析

**任务：** 实现函数获取最大缓存级别:

```cpp
u32 get_max_cache_level() {
    u64 clidr;
    asm volatile("mrs %0, clidr_el1" : "=r"(clidr));
    // TODO: 从 LoC 字段获取
}
```

---

## 练习 4: 缓存刷新范围

**问题：** 刷新一段内存的缓存时，为什么需要对齐到缓存行边界？

```cpp
void clean_range(void* addr, usize size) {
    u64 start = (u64)addr & ~(line_size - 1);  // 为什么要对齐？
    // ...
}
```

---

## 练习 5: I-Cache 刷新

**问题：** 修改代码后需要刷新指令缓存。实现:

```cpp
void flush_code(void* addr, usize size) {
    // 1. Clean D-Cache (确保代码写入内存)
    // 2. Invalidate I-Cache (确保 CPU 重新读取)
}
```

---

## 练习 6: 缓存一致性

**问题：** 设备 DMA 写入内存后，CPU 需要做什么？

```cpp
void after_dma_write(void* buf, usize size) {
    // TODO: CPU 需要刷新什么？
}
```

---

## 练习 7: 页表遍历

**任务：** 实现完整的页表遍历函数:

```cpp
u64 walk_pgtable(u64* pgd, u64 va) {
    u64 pgd_idx = (va >> 30) & 0x1FF;
    u64 pud_idx = (va >> 21) & 0x1FF;
    u64 pmd_idx = (va >> 12) & 0x1FF;
    u64 pte_idx = (va >> 3) & 0x1FF;  // 这个对吗？

    // TODO: 完成实现
}
```

---

## 练习 8: 缓存策略

**问题：** 解释 VIPT 和 PIPT 的区别:
1. VIPT (Virtual Index Physical Tag)
2. PIPT (Physical Index Physical Tag)

**提示：** 考虑别名问题 (aliasing)。

---

## 练习 9: PoC vs PoU

**问题：** 以下操作应该用 PoC 还是 PoU？

1. 修改代码后刷新指令缓存: `dc cvau` 还是 `dc cvac`？
2. DMA 准备读取数据: `dc cvac` 还是 `dc cvau`？

---

## 练习 10: 缓存性能

**问题：** 为什么缓存行大小影响性能？

**分析：**
- 缓存行太小 → 空间局部性差
- 缓存行太大 → 缓存颠簸 (thrashing)

**任务：** 设计一个测试程序测量不同访问模式的缓存性能。