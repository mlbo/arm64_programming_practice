# 练习题

## 练习 1: 原子自增

实现一个原子自增函数，返回自增后的值：

```asm
atomic_inc:
    // x0 = addr
    // 返回 x0 = 自增后的值
```

## 练习 2: 原子位操作

实现原子设置和清除位：

```asm
// 原子设置位
atomic_set_bit:
    // x0 = addr, x1 = bit_position

// 原子清除位
atomic_clear_bit:
    // x0 = addr, x1 = bit_position
```

## 练习 3: 检测 LSE 支持

编写代码检测 CPU 是否支持 LSE 原子指令扩展。

提示：读取 ID_AA64ISAR0_EL1 寄存器的 Atomic 域。

## 练习 4: 内存屏障

为什么在多核环境中需要 DMB 指令？在什么位置添加 DMB？

## 练习 5: 性能对比

比较 LDXR/STXR 循环和 LSE 原子指令的性能差异。