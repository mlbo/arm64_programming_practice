# Lab 23-2: SVE 交错加载/存储

## 实验目标

- 掌握 SVE 的交错加载/存储指令 (ld3b/st3b)
- 理解 SVE 与 NEON 交错操作的区别
- 实现 RGB/BGR 转换的 SVE 版本

## 实验内容

本实验演示如何使用 SVE 交错加载/存储指令处理 RGB 图像数据。

### 关键技术点

1. **交错指令**: `ld3b`, `st3b` (3 通道字节)
2. **Predicate 控制**: 处理任意长度数据
3. **VLA 模式**: 代码与向量长度无关

## 构建与运行

```bash
make && ./sve_interleaved_test
```

## 预期输出

```
RGB/BGR conversion test (SVE)
Pixels: 4194304
Result verified: PASSED
```

## 参考文献

- ARM SVE Architecture Reference Manual
- SVE ACLE Intrinsics Reference