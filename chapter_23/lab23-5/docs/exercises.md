# 练习题

## 基础练习

### 1. 扩展到 8x8 矩阵

修改代码支持 8x8 矩阵乘法。

**提示**: 将 8x8 矩阵分解为 4 个 4x4 子矩阵。

### 2. 矩阵加法

使用 SVE 实现矩阵加法：

```cpp
void matrix_add_4x4_sve(const float* a, const float* b, float* c);
```

## 进阶练习

### 3. 通用矩阵乘法

实现支持任意大小的矩阵乘法：

```cpp
void matrix_mul_sve(const float* a, const float* b, float* c,
                    int m, int n, int k);
```

### 4. 分块优化

实现分块矩阵乘法以提高缓存效率：

```cpp
void matrix_mul_blocked_sve(const float* a, const float* b, float* c,
                            int m, int n, int k, int block_size);
```

### 5. 性能对比

编写基准测试比较：
- 标量版本
- NEON 版本
- SVE 版本

测试不同矩阵大小。

## 思考题

1. 为什么 4x4 矩阵使用 `ptrue p0.s, vl4`？
2. SVE 矩阵乘法相比 NEON 有什么优势？
3. 如何处理非方阵的矩阵乘法？