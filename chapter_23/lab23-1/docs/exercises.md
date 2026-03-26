# 练习题

## 基础练习

### 1. 打印向量长度

编写程序打印当前系统的 SVE 向量长度（以 bit 为单位）。

### 2. Predicate 可视化

编写函数将 Predicate 寄存器的值打印为二进制字符串。

**提示**: 使用 `svptrue_b8()` 创建全 1 predicate，然后逐位检查。

## 进阶练习

### 3. 向量加法

使用 SVE intrinsics 实现两个数组的相加：

```cpp
void add_arrays_sve(const float* a, const float* b, float* c, int n);
```

### 4. 向量初始化

使用 SVE 实现数组初始化：

```cpp
void fill_array_sve(float* arr, float value, int n);
```

### 5. 条件赋值

使用 SVE 实现：当 a[i] > threshold 时，c[i] = a[i]，否则 c[i] = 0。

```cpp
void conditional_assign_sve(
    const float* a, float* c, float threshold, int n
);
```

**提示**: 使用 `svcmpgt_f32` 创建 predicate。

## 思考题

1. 为什么 SVE 的向量长度设计为可变的？
2. Predicate 编程相比分支有什么优势？
3. VLA 编程如何保证代码在不同硬件上的正确性？

## 参考资料

- ARM SVE ACLE Specification
- "Programming with SVE" - ARM Learn