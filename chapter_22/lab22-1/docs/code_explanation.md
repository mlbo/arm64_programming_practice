# 代码说明

## 文件结构

```
lab22-1/
├── include/
│   └── neon_utils.hpp    # NEON 工具函数声明
├── src/
│   ├── neon_rgb_bgr.cpp  # RGB/BGR 转换实现
│   └── main.cpp          # 测试主程序
└── Makefile
```

## 核心函数解析

### 1. 标量版本 (C 语言实现)

```cpp
void rgb_to_bgr_scalar(const u8* src, u8* dst, usize pixels) {
    for (usize i = 0; i < pixels; ++i) {
        dst[i * 3 + 0] = src[i * 3 + 2];  // B = src 的 R
        dst[i * 3 + 1] = src[i * 3 + 1];  // G = src 的 G
        dst[i * 3 + 2] = src[i * 3 + 0];  // R = src 的 B
    }
}
```

每次处理 1 个像素，需要 3 次内存访问。

### 2. NEON Intrinsics 版本

```cpp
void rgb_to_bgr_neon(const u8* src, u8* dst, usize pixels) {
    usize i = 0;

    // 每次处理 16 像素 (48 字节)
    for (; i + 15 < pixels; i += 16) {
        // 交错加载 RGB
        uint8x16x3_t rgb = vld3q_u8(src + i * 3);

        // 交换 R 和 B
        uint8x16x3_t bgr;
        bgr.val[0] = rgb.val[2];  // B
        bgr.val[1] = rgb.val[1];  // G
        bgr.val[2] = rgb.val[0];  // R

        // 交错存储 BGR
        vst3q_u8(dst + i * 3, bgr);
    }

    // 处理剩余像素
    for (; i < pixels; ++i) {
        dst[i * 3 + 0] = src[i * 3 + 2];
        dst[i * 3 + 1] = src[i * 3 + 1];
        dst[i * 3 + 2] = src[i * 3 + 0];
    }
}
```

每次处理 16 像素，只需 2 次向量内存访问 (1 次加载 + 1 次存储)。

### 3. NEON 汇编版本

```cpp
void rgb_to_bgr_asm(const u8* src, u8* dst, usize pixels) {
    usize count = pixels * 3;
    usize offset = 0;

    asm volatile (
        "1: ld3 {v0.16b, v1.16b, v2.16b}, [%[src]], #48 \n"
        "mov v3.16b, v0.16b\n"
        "mov v0.16b, v2.16b\n"
        "mov v2.16b, v3.16b\n"
        "st3 {v0.16b, v1.16b, v2.16b}, [%[dst]], #48\n"
        "add %[offset], %[offset], #48\n"
        "cmp %[offset], %[count]\n"
        "bne 1b\n"
        : [dst] "+r"(dst), [src] "+r"(src), [offset] "+r"(offset)
        : [count] "r" (count)
        : "memory", "v0", "v1", "v2", "v3"
    );
}
```

### 指令解析

| 指令 | 功能 |
|------|------|
| `ld3 {v0, v1, v2}, [src], #48` | 交错加载 48 字节到 3 个向量，并更新地址 |
| `mov v3, v0` | 复制向量 |
| `st3 {v0, v1, v2}, [dst], #48` | 交错存储 3 个向量，并更新地址 |

## 性能对比

| 版本 | 优势 | 劣势 |
|------|------|------|
| 标量 C | 简单易懂，可移植 | 最慢 |
| NEON Intrinsics | 较快，可读性好 | 需要理解 NEON 类型 |
| NEON 汇编 | 最快，完全控制 | 难以维护，不可移植 |

典型的性能提升比例：
- NEON vs 标量: 3-8x 加速
- 汇编 vs Intrinsics: 1-1.2x 加速