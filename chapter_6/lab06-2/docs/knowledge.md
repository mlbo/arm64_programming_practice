# 知识点：数据处理指令

## 1. 带进位运算

### ADDS 和 ADC

```asm
adds x0, x1, x2    // x0 = x1 + x2，设置标志位
adc x3, x4, x5     // x3 = x4 + x5 + C
```

### CMP 和 SBC

```asm
cmp x0, x1         // 设置标志位
sbc x0, xzr, xzr   // x0 = 0 - 0 - !C
```

## 2. 位段操作

### BFI (Bit Field Insert)

```asm
bfi x0, x1, #8, #4   // 将 x1 的低 4 位插入 x0 的第 8-11 位
```

### UBFX (Unsigned Bit Field Extract)

```asm
ubfx x0, x1, #4, #8  // 提取 x1 的第 4-11 位，零扩展
```

### SBFX (Signed Bit Field Extract)

```asm
sbfx x0, x1, #4, #8  // 提取 x1 的第 4-11 位，符号扩展
```

## 3. 多精度运算

128 位加法示例：

```asm
adds x0, x2, x4    // 低 64 位加法，设置进位
adc x1, x3, x5     // 高 64 位加法，包含进位
```