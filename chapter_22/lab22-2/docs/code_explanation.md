# 代码说明

## 核心函数解析

### 1. 标量版本

```cpp
void matrix_mul_4x4_scalar(const float* a, const float* b, float* c) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            c[4 * j + i] = 0;
            for (int k = 0; k < 4; ++k) {
                c[4 * j + i] += a[4 * k + i] * b[4 * j + k];
            }
        }
    }
}
```

### 2. NEON Intrinsics 版本

```cpp
void matrix_mul_4x4_neon(const float* a, const float* b, float* c) {
    // Load columns of matrix B
    float32x4_t b0 = vld1q_f32(b);      // B 的第 0 列
    float32x4_t b1 = vld1q_f32(b + 4);  // B 的第 1 列
    float32x4_t b2 = vld1q_f32(b + 8);  // B 的第 2 列
    float32x4_t b3 = vld1q_f32(b + 12); // B 的第 3 列

    // Compute each row of result matrix
    for (int i = 0; i < 4; ++i) {
        float32x4_t a_row = vld1q_f32(a + i * 4);

        // C[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + ...
        float32x4_t r = vmulq_laneq_f32(b0, a_row, 0);
        r = vfmaq_laneq_f32(r, b1, a_row, 1);
        r = vfmaq_laneq_f32(r, b2, a_row, 2);
        r = vfmaq_laneq_f32(r, b3, a_row, 3);

        vst1q_f32(c + i * 4, r);
    }
}
```

### 3. 汇编版本

```cpp
void matrix_mul_4x4_asm(const float* a, const float* b, float* c) {
    asm volatile (
        // Load matrix A (4 columns)
        "ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[a]]\n"
        // Load matrix B (4 columns)
        "ld1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[b]]\n"

        // Initialize result registers
        "movi v8.4s,  #0\n"
        "movi v9.4s,  #0\n"
        "movi v10.4s, #0\n"
        "movi v11.4s, #0\n"

        // Compute C column 0
        "fmla v8.4s, v0.4s, v4.s[0]\n"
        "fmla v8.4s, v1.4s, v4.s[1]\n"
        "fmla v8.4s, v2.4s, v4.s[2]\n"
        "fmla v8.4s, v3.4s, v4.s[3]\n"

        // ... (similar for columns 1-3)

        // Store result
        "st1 {v8.4s, v9.4s, v10.4s, v11.4s}, [%[c]]\n"
        :
        : [a] "r" (a), [b] "r" (b), [c] "r" (c)
        : "memory", "v0", "v1", ..., "v11"
    );
}
```

## 指令详解

### ld1 指令

```asm
ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [x0]
```
从内存连续加载 4 个 128-bit 向量到 V0-V3。

### fmla 指令

```asm
fmla Vd.4s, Vn.4s, Vm.s[lane]
```
- `Vd = Vd + Vn × Vm[lane]`
- 将 `Vm` 的第 `lane` 个元素广播后与 `Vn` 相乘
- 结果加到 `Vd`

## 数据布局

内存中的矩阵存储（列主序）：

```
地址偏移:  0   4   8   12  16  20  24  28  ...
元素:    [00][10][20][30][01][11][21][31]...
          |----第0列----|----第1列----|
```