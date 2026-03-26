# Lab 23-5: SVE 矩阵乘法

## 实验目标

- 使用 SVE 实现 4x4 矩阵乘法
- 理解 SVE 浮点运算指令
- 对比 SVE 与 NEON 矩阵乘法

## 实验内容

本实验演示如何使用 SVE 指令优化 4x4 单精度浮点矩阵乘法。

### 关键技术点

1. **ptrue 指令**: 设置固定长度 predicate
2. **fmul/fmla**: 浮点乘法和融合乘加
3. **向量元素广播**: 使用 lane 索引

## 构建与运行

```bash
make && ./sve_matrix_test
```

## 预期输出

```
Matrix A:
...
Matrix B:
...
C result (scalar):
...
ASM result:
...
ASM equal to C: yes
ASM faster than scalar: X.XXx
```