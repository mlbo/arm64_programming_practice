# 知识点: ADDS/CMP/ADC

## 条件标志位 (NZCV)

| 标志 | 名称 | 含义 |
|------|------|------|
| N | Negative | 结果为负 |
| Z | Zero | 结果为零 |
| C | Carry | 无符号溢出/进位 |
| V | Overflow | 有符号溢出 |

## ADDS

```asm
adds x0, x1, x2    // x0 = x1 + x2, 设置标志位
```

- 无符号溢出: C=1
- 有符号溢出: V=1

## ADC

```asm
adc x0, x1, x2     // x0 = x1 + x2 + C
```

用于大数加法:
```asm
adds x0, x0, x1    // 低 64 位加
adc  x2, x2, xzr   // 捕获进位到高 64 位
```

## CMP

```asm
cmp x1, x2         // 计算 x1 - x2, 设置标志位
```

等价于:
```asm
subs xzr, x1, x2
```

## 常用条件码

| 条件码 | 含义 | 标志 |
|--------|------|------|
| EQ | Equal | Z=1 |
| NE | Not Equal | Z=0 |
| GT | Greater Than | Z=0 && N=V |
| LT | Less Than | N!=V |
| GE | Greater or Equal | N=V |
| LE | Less or Equal | Z=1 \|\| N!=V |