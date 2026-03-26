# 代码说明

## 核心函数解析

### SVE 汇编实现

```cpp
void matrix_mul_4x4_sve(const float* a, const float* b, float* c) {
    asm volatile (
        "ptrue p0.s, vl4\n"

        // 加载矩阵 A 到 Z0-Z3
        "ld1w {z0.s}, p0/z, [%[a]]\n"
        "incw %[a], VL4, MUL #4\n"
        "ld1w {z1.s}, p0/z, [%[a]]\n"
        "incw %[a], VL4, MUL #4\n"
        "ld1w {z2.s}, p0/z, [%[a]]\n"
        "incw %[a], VL4, MUL #4\n"
        "ld1w {z3.s}, p0/z, [%[a]]\n"

        // 加载矩阵 B 到 Z4-Z7
        "ld1w {z4.s}, p0/z, [%[b]]\n"
        "incw %[b], VL4, MUL #4\n"
        "ld1w {z5.s}, p0/z, [%[b]]\n"
        "incw %[b], VL4, MUL #4\n"
        "ld1w {z6.s}, p0/z, [%[b]]\n"
        "incw %[b], VL4, MUL #4\n"
        "ld1w {z7.s}, p0/z, [%[b]]\n"

        // 计算结果列 0
        "fmul z8.s, z0.s, z4.s[0]\n"
        "fmla z8.s, z1.s, z4.s[1]\n"
        "fmla z8.s, z2.s, z4.s[2]\n"
        "fmla z8.s, z3.s, z4.s[3]\n"

        // ... 计算列 1-3 ...

        // 存储结果
        "st1w {z8.s}, p0, [%[c]]\n"
        // ...
        :
        : [a] "r" (a), [b] "r" (b), [c] "r" (c)
        : "memory", "z0", ..., "z11", "p0"
    );
}
```

## 指令详解

### ptrue p0.s, vl4

设置 predicate p0，使其激活 4 个 word lane：
```
p0: [1][1][1][1][0][0]...
```

### ld1w {z0.s}, p0/z, [x0]

从 x0 指向的地址加载 4 个 word 到 z0：
```
z0.s = [word0][word1][word2][word3]
```

### incw x0, VL4, MUL #4

增加地址，跳过 4 × 4 = 16 words = 64 bytes。

### fmul z8.s, z0.s, z4.s[0]

将 z0 的所有元素与 z4 的第 0 个元素相乘：
```
z8[i] = z0[i] × z4[0]
```

### fmla z8.s, z1.s, z4.s[1]

融合乘加：
```
z8[i] = z8[i] + z1[i] × z4[1]
```

## 内存布局

使用列主序存储：

```
地址偏移:  0   4   8   12  16  20  24  28  ...
元素:    [00][10][20][30][01][11][21][31]...
          |----第0列----|----第1列----|
```

这与 NEON 版本保持一致。