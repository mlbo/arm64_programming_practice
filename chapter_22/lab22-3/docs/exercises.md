# 练习题

## 基础练习

### 1. 分析向量化报告

编译以下代码并查看向量化报告：

```cpp
void process(float* a, float* b, float* c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] * b[i] + a[i];
    }
}
```

- 哪些循环被向量化了？
- 为什么？

### 2. 修复向量化失败

以下代码无法向量化，请修复：

```cpp
void process(float* a, float* b, int n) {
    for (int i = 0; i < n; ++i) {
        b[i] = a[i] + b[i-1];  // 数据依赖
    }
}
```

## 进阶练习

### 3. restrict 关键字对比

实现两个版本的函数，对比有无 `restrict` 关键字的性能差异：

```cpp
// 版本 1: 无 restrict
void add_v1(float* a, float* b, float* c, int n);

// 版本 2: 有 restrict
void add_v2(float* __restrict__ a,
            float* __restrict__ b,
            float* __restrict__ c, int n);
```

### 4. 分支优化

以下代码因复杂分支难以向量化，请优化：

```cpp
void clamp(float* data, float min, float max, int n) {
    for (int i = 0; i < n; ++i) {
        if (data[i] < min) data[i] = min;
        else if (data[i] > max) data[i] = max;
    }
}
```

**提示**: 考虑使用三元运算符或数学函数。

### 5. 循环展开

手动展开循环并对比性能：

```cpp
// 原始循环
void process_v1(float* a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = a[i] * 2 + 1;
    }
}

// 展开版本
void process_v2(float* a, int n) {
    // 手动展开 4 次
}
```

## 思考题

1. 为什么有时手动 NEON 代码比自动向量化慢？
2. 什么情况下应该优先使用自动向量化？
3. 如何在不牺牲可读性的前提下帮助编译器优化？

## 参考答案

部分练习位于 `src/exercise_solutions.cpp`。