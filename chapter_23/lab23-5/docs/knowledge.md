# SVE 矩阵乘法知识

## 4x4 矩阵乘法

### 数学公式

对于 C = A × B：

```
C[i][j] = Σ(k=0 to 3) A[i][k] × B[k][j]
```

### SVE 优化策略

与 NEON 类似，但使用 SVE 指令：
1. 加载矩阵到 Z 寄存器
2. 使用 FMLA (融合乘加) 计算
3. 存储结果

## 关键 SVE 指令

### ptrue - 设置 Predicate

```asm
ptrue p0.s, vl4    // 设置 4 个 word lane 为激活
```

`vl4` 表示向量长度为 4 个元素（适合 4x4 矩阵）。

### ld1w / st1w - 加载/存储字

```asm
ld1w {z0.s}, p0/z, [x0]     // 加载向量
st1w {z0.s}, p0, [x1]       // 存储向量
```

### fmul / fmla - 浮点运算

```asm
fmul z8.s, z0.s, z4.s[0]    // z8 = z0 × z4[0]
fmla z8.s, z1.s, z4.s[1]    // z8 += z1 × z4[1]
```

### incw - 增加向量长度

```asm
incw x0, VL4, MUL #4    // x0 += 4 * 4 words (16 words = 64 bytes)
```

## SVE vs NEON 矩阵乘法

| 特性 | NEON | SVE |
|------|------|-----|
| 向量长度 | 固定 128-bit | 可变 |
| 寄存器 | V0-V31 | Z0-Z31 |
| Predicate | 无 | P0-P15 |
| 加载指令 | ld1 | ld1w |

对于 4x4 矩阵，SVE 没有明显的性能优势，但代码更清晰。

## 代码实现

### 汇编版本

```asm
matrix_mul_4x4_sve:
    ptrue p0.s, vl4        // 4 个 word lane

    // 加载矩阵 A
    ld1w {z0.s}, p0/z, [%[a]]
    incw %[a], VL4, MUL #4
    ld1w {z1.s}, p0/z, [%[a]]
    incw %[a], VL4, MUL #4
    ld1w {z2.s}, p0/z, [%[a]]
    incw %[a], VL4, MUL #4
    ld1w {z3.s}, p0/z, [%[a]]

    // 加载矩阵 B
    ld1w {z4.s}, p0/z, [%[b]]
    // ... 类似 ...

    // 计算结果
    fmul z8.s, z0.s, z4.s[0]
    fmla z8.s, z1.s, z4.s[1]
    fmla z8.s, z2.s, z4.s[2]
    fmla z8.s, z3.s, z4.s[3]

    // 存储结果
    st1w {z8.s}, p0, [%[c]]
    // ...
```

## 性能考虑

对于小矩阵（4x4）：
- SVE 与 NEON 性能接近
- SVE 代码稍简洁（使用 ptrue）
- 两者都比标量快很多

对于大矩阵：
- 需要考虑分块 (blocking)
- SVE 的向量长度可变性可能带来额外优化机会