# 知识点: CMP/SBC

## SBC 指令

```asm
sbc xd, xn, xm    // xd = xn - xm - !C
```

注意: SBC 使用的是 C 标志的**反值** (!C)

## 比较结果转换

| 比较 | C | !C | SBC 结果 |
|------|---|-----|----------|
| a >= b | 1 | 0 | 0 |
| a < b | 0 | 1 | -1 (0xfff...) |

## 技巧

`cmp + sbc xzr, xzr` 组合可以快速生成比较结果的掩码:

```asm
cmp x0, x1
sbc x2, xzr, xzr
// x2 = 0 如果 x0 >= x1
// x2 = -1 如果 x0 < x1
```

可用于无分支的条件选择:

```asm
cmp x0, x1
sbc x2, xzr, xzr
and x2, x2, x3    // 如果 x0 < x1，x2 = x3，否则 0
```