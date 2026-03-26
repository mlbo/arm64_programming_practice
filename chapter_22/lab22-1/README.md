# Lab 22-1: NEON RGB/BGR 转换

## 实验目标

- 掌握 NEON intrinsics 编程基础
- 理解 NEON 交错加载/存储指令 (vld3q_u8, vst3q_u8)
- 对比标量代码与 NEON 优化代码的性能差异

## 实验内容

本实验演示如何使用 NEON SIMD 指令实现 RGB24 到 BGR24 的图像格式转换。

### 关键技术点

1. **NEON 寄存器**: V0-V31，每个 128-bit
2. **向量类型**: `uint8x16_t` (16 个 8-bit 无符号整数)
3. **交错加载**: `vld3q_u8` 一次加载 3 个向量 (R, G, B 分离)
4. **交错存储**: `vst3q_u8` 一次存储 3 个向量

## 构建与运行

```bash
# 编译
make

# 运行
./neon_rgb_bgr_test
```

## 预期输出

```
C version spent time: XXX ms
NEON intrinsics spent time: XXX ms
ASM version spent time: XXX ms
bgr result (XXXXX) is identical
ASM faster than C: X.XX
ASM faster than NEON: X.XX
```

## 参考文献

- ARM NEON Intrinsics Reference
- ARM Architecture Reference Manual ARMv8-A