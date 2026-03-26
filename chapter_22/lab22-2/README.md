# Lab 22-2: NEON 矩阵乘法

## 实验目标

- 掌握 NEON 浮点运算 intrinsics
- 理解 FMA (Fused Multiply-Add) 指令的使用
- 学习矩阵乘法的 SIMD 优化技巧

## 实验内容

本实验演示如何使用 NEON 指令优化 4x4 单精度浮点矩阵乘法。

### 关键技术点

1. **浮点向量类型**: `float32x4_t` (4 个 float)
2. **FMA 指令**: `vfmaq_laneq_f32` (融合乘加)
3. **矩阵分块**: 利用 NEON 的 4-lane 特性

## 构建与运行

```bash
make && ./neon_matrix_test
```

## 预期输出

```
Matrix A:
0.XX 0.XX ...
...
Matrix B:
...
C result (scalar):
...
NEON result:
...
ASM result:
...
Neon equal to C: yes
Asm equal to C: yes
ASM faster than C: X.XX
```

## 参考文献

- ARM NEON Intrinsics Reference
- "Optimizing Matrix Multiplication" - ARM Developer