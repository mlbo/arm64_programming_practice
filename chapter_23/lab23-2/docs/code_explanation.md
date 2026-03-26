# 代码说明

## 核心函数解析

### 1. SVE Intrinsics 版本

```cpp
void rgb_to_bgr_sve_intr(const u8* src, u8* dst, usize bytes) {
    u64 offset = 0;

    while (offset < bytes) {
        svbool_t pg = svwhilelt_b8(offset, bytes);

        // Load interleaved RGB
        svuint8x3_t rgb = svld3_u8(pg, src + offset);

        // Swap R and B channels
        svuint8x3_t bgr = svcreate3_u8(
            svget3_u8(rgb, 2),  // B from original R
            svget3_u8(rgb, 1),  // G unchanged
            svget3_u8(rgb, 0)   // R from original B
        );

        // Store interleaved BGR
        svst3_u8(pg, dst + offset, bgr);

        // Move to next chunk (3 vectors worth of bytes)
        offset += svcntb() * 3;
    }
}
```

### 2. SVE 汇编版本

```cpp
void rgb_to_bgr_sve_asm(const u8* src, u8* dst, usize bytes) {
    asm volatile (
        "mov     x3, #0\n"
        "whilelo p0.b, x3, %2\n"

        "1:\n"
        "ld3b    {z4.b - z6.b}, p0/z, [%0, x3]\n"
        "mov     z1.d, z6.d\n"
        "mov     z2.d, z5.d\n"
        "mov     z3.d, z4.d\n"
        "st3b    {z1.b - z3.b}, p0, [%1, x3]\n"
        "incb    x3, all, mul #3\n"
        "whilelo p0.b, x3, %2\n"
        "b.any   1b\n"
        :
        : "r"(src), "r"(dst), "r"(bytes)
        : "memory", "x3", "p0",
          "z1", "z2", "z3", "z4", "z5", "z6"
    );
}
```

## 关键指令详解

### svld3_u8 / ld3b

```cpp
svuint8x3_t rgb = svld3_u8(pg, ptr);
```

等效汇编：
```asm
ld3b {z0.b, z1.b, z2.b}, pg/z, [ptr]
```

加载后：
- `svget3_u8(rgb, 0)` → z0.b (R 通道)
- `svget3_u8(rgb, 1)` → z1.b (G 通道)
- `svget3_u8(rgb, 2)` → z2.b (B 通道)

### svst3_u8 / st3b

```cpp
svst3_u8(pg, ptr, bgr);
```

等效汇编：
```asm
st3b {z0.b, z1.b, z2.b}, pg, [ptr]
```

### svcntb / incb

```cpp
u64 vl = svcntb();      // 获取字节向量长度
// 汇编: incb x3, all, mul #3  // x3 += 3 * VL
```

## 性能对比

| 版本 | 特点 |
|------|------|
| C 标量 | 最慢，但最简单 |
| SVE Intrinsics | VLA，可读性好 |
| SVE 汇编 | 最快，完全控制 |

典型加速比：SVE vs C 标量 = 5-10x