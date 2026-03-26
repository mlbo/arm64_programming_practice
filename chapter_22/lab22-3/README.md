# Lab 22-3: 自动向量化对比

## 实验目标

- 理解编译器自动向量化的原理
- 学习如何帮助编译器进行向量化优化
- 对比手动 NEON 优化与自动向量化的性能

## 实验内容

本实验对比编译器自动向量化与手动 NEON 优化的差异，展示如何编写"对向量化友好"的代码。

### 关键技术点

1. **编译器选项**: `-ftree-vectorize`, `-fopt-info-vec`
2. **向量化友好的代码模式**
3. **限制向量化的因素**

## 构建与运行

```bash
make && ./auto_vectorization_test
```

## 查看向量化报告

```bash
# 查看哪些循环被向量化
g++ -O3 -fopt-info-vec -c src/test.cpp

# 查看详细的向量化信息
g++ -O3 -fopt-info-vec-optimized src/test.cpp
```

## 预期输出

```
Scalar version: XXX ms
Auto-vectorized: XXX ms
NEON manual: XXX ms
Auto-vec speedup: X.XXx
NEON speedup: X.XXx
```

## 参考文献

- GCC Auto-vectorization: https://gcc.gnu.org/projects/tree-ssa/vectorization.html
- "Auto-vectorization in GCC" - GNU Cauldron