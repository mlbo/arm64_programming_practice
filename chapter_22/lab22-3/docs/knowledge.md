# 自动向量化知识

## 编译器自动向量化概述

现代编译器 (GCC, Clang) 可以自动将循环转换为 SIMD 指令，这个过程称为自动向量化 (Auto-vectorization)。

### 启用自动向量化的编译选项

```bash
# 基本向量化 (O2 及以上默认启用)
-O2 -ftree-vectorize

# 高级优化
-O3 -ffast-math -funroll-loops

# 特定架构优化
-march=armv8-a+simd
```

### 查看向量化报告

```bash
# 显示被向量化的循环
-fopt-info-vec

# 显示所有向量化相关信息
-fopt-info-vec-all

# 只显示优化成功的信息
-fopt-info-vec-optimized

# 显示优化失败的原因
-fopt-info-vec-missed
```

## 向量化友好的代码模式

### 1. 简单的独立循环

```cpp
// 容易向量化
void add_arrays(float* a, float* b, float* c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}
```

### 2. 已知边界的循环

```cpp
// 循环次数已知，更容易向量化
void process_fixed(float* data) {
    for (int i = 0; i < 1024; ++i) {  // 编译时常量
        data[i] *= 2.0f;
    }
}
```

### 3. 无数据依赖

```cpp
// 好例子：无数据依赖
void scale(float* a, float factor, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] *= factor;  // 每次迭代独立
    }
}

// 坏例子：有数据依赖
void shift(float* a, int n) {
    for (int i = 1; i < n; ++i) {
        a[i] = a[i-1];  // 依赖前一次迭代
    }
}
```

### 4. 连续内存访问

```cpp
// 好例子：连续访问
void sum_rows(float* matrix, float* sums, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        float sum = 0;
        for (int j = 0; j < cols; ++j) {
            sum += matrix[i * cols + j];  // 连续读取
        }
        sums[i] = sum;
    }
}
```

## 限制向量化的因素

### 1. 指针别名 (Pointer Aliasing)

```cpp
// 编译器无法确定 a, b 是否指向同一内存
void unknown(float* a, float* b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] + 1;  // 可能向量化失败
    }
}

// 解决方案：使用 restrict 关键字
void known(float* __restrict__ a, float* __restrict__ b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] + 1;  // 保证向量化
    }
}
```

### 2. 复杂的控制流

```cpp
// 难以向量化：分支复杂
void complex_control(float* a, int n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] > 0 && a[i] < 10) {
            a[i] = 1;
        } else if (a[i] < 0) {
            a[i] = -1;
        } else {
            a[i] = 0;
        }
    }
}
```

### 3. 函数调用

```cpp
// 难以向量化：循环内有函数调用
void with_call(float* a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = sinf(a[i]);  // 编译器可能无法向量化
    }
}

// 解决方案：使用 SIMD 数学库
// 或使用编译器提供的批量数学函数
```

### 4. 不对齐的内存访问

```cpp
// 可能影响向量化效率
void unaligned(float* a, int n) {
    // 如果 a 不是 16 字节对齐，可能效率较低
    for (int i = 0; i < n; ++i) {
        a[i] *= 2;
    }
}

// 解决方案：使用对齐属性
void aligned(float* __restrict__ a, int n) __attribute__((assume_aligned(16)));
```

## 如何帮助编译器

1. **使用 restrict 关键字** 告知编译器指针不重叠
2. **使用 const 关键字** 帮助编译器分析
3. **避免循环内的函数调用**
4. **保持简单控制流**
5. **使用编译时常量**
6. **手动添加编译器提示**

```cpp
// 编译器提示：循环次数是 4 的倍数
void with_hint(float* a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] *= 2;
    }
}
// 编译选项: -funroll-loops