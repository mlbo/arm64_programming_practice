# 练习题

## 练习 1: 128 位加法

实现一个 128 位加法函数：

```asm
// x0, x1 = x2, x3 + x4, x5
add_128:
    // TODO: 实现两个 128 位数的加法
```

## 练习 2: 位掩码

使用 BFI 指令实现以下功能：

```c
uint32_t set_bits(uint32_t val, uint32_t bits, int pos, int width) {
    // 将 bits 的低 width 位插入 val 的 pos 位置
}
```

## 练习 3: 符号扩展

分析 UBFX 和 SBFX 的区别。给定 x1 = 0x80，执行以下指令后 x0 的值分别是多少？

```asm
ubfx x0, x1, #0, #8
sbfx x0, x1, #0, #8
```

## 练习 4: 溢出检测

如何使用 ADDS 检测有符号数加法溢出？

## 练习 5: 系统寄存器

使用 UBFX 读取 ID_AA64ISAR0_EL1 寄存器，判断 CPU 是否支持以下特性：
- AES 指令
- SHA1 指令
- CRC32 指令