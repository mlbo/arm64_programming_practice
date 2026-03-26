# Lab 23-6: SVE FFR (First Fault Register)

## 实验目标

- 理解 First Fault Register 的原理
- 掌握 ldff1 系列指令的使用
- 学习如何安全处理越界内存访问

## 实验内容

本实验深入讲解 SVE 的 FFR 机制，演示如何在不知道数据长度的情况下安全处理内存访问。

### 关键技术点

1. **FFR**: 记录首次访存失败位置
2. **ldff1b**: First-Fault 加载指令
3. **rdffrs**: 读取 FFR 状态
4. **setffr**: 初始化 FFR

## 构建与运行

```bash
make && ./sve_ffr_test
```

## 预期输出

```
=== FFR Basic Test ===
FFR initialized successfully
First-fault load succeeded

=== FFR Boundary Test ===
Accessed up to boundary correctly
FFR detected page boundary

All tests passed!
```