# 知识点: ANDS 与 Z 标志位

## ANDS 指令

```asm
ands xd, xn, xm    // xd = xn & xm, 设置标志位
```

ANDS 执行按位与操作并设置条件标志位。

## Z 标志位

| 结果 | Z |
|------|---|
| 0 | 1 |
| 非 0 | 0 |

## NZCV 寄存器

```
Bit 31: N (Negative)
Bit 30: Z (Zero)
Bit 29: C (Carry)
Bit 28: V (Overflow)
```

读取:
```asm
mrs x0, nzcv
```

## 测试示例

```asm
mov    x1, #0x3
mov    x2, #0
ands   x3, x1, x2    // x3 = 0, Z = 1

mov    x2, #2
ands   x3, x1, x2    // x3 = 2, Z = 0
```

## 应用

常用于测试位:
```asm
ands   x0, x0, #(1 << 5)   // 测试 bit 5
b.eq   bit_not_set          // Z=1 表示该位为 0
```