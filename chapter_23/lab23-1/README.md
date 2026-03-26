# Lab 23-1: SVE 基础与 Predicate

## 实验目标

- 理解 SVE 的向量长度无关编程模型
- 掌握 Predicate 寄存器的使用
- 学习基本的 SVE 指令

## 实验内容

本实验介绍 SVE (Scalable Vector Extension) 的基础知识，包括向量长度检测和 Predicate 操作。

### 关键技术点

1. **向量长度可变**: SVE 支持 128-2048 bit
2. **Predicate 寄存器**: P0-P15 (每 bit 控制一个向量元素)
3. **VLA 编程**: 代码与向量长度无关

## 构建与运行

```bash
make && ./sve_basic_test
```

## 预期输出

```
SVE is supported: yes
Vector length: XXX bits
Predicate pattern test passed
```

## 参考文献

- ARM SVE Architecture Reference Manual
- "The Scalable Vector Extension (SVE)" - ARM Developer