# 练习题

## 基础练习

### 1. 扩展到 8x8 矩阵

修改代码，实现 8x8 矩阵乘法。

**提示**: 将 8x8 矩阵分解为 4 个 4x4 子矩阵。

### 2. 添加矩阵加法

实现 NEON 版本的矩阵加法：

```cpp
void matrix_add_4x4_neon(const float* a, const float* b, float* c);
```

## 进阶练习

### 3. 实现矩阵转置

使用 NEON 实现 4x4 矩阵转置。

**提示**: 使用 `vtrnq_f32` 和 `vzipq_f32` 指令。

### 4. 实现矩阵-向量乘法

```cpp
void matrix_vec_mul_4x4_neon(
    const float* matrix,
    const float* vec,
    float* result
);
```

### 5. 性能计数

使用 `perf` 工具分析 NEON 指令的执行效率：

```bash
perf stat -e instructions,cycles ./neon_matrix_test
```

比较标量和 NEON 版本的 IPC (Instructions Per Cycle)。

## 思考题

1. 为什么矩阵乘法使用列主序存储更高效？
2. FMA 指令相比分开的乘法和加法有什么优势？
3. 如何避免矩阵乘法中的缓存失效？

## 参考资料

- ARM Cortex-A Series Programmer's Guide
- "Optimizing Subroutines in Assembly Language" by Agner Fog