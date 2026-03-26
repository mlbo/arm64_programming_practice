# SVE 交错加载/存储知识

## 交错指令概述

SVE 提供了多种交错加载/存储指令，用于处理结构化数据。

### 指令格式

```
ldN{b|h|w|d}  // 加载 N 个向量，每向量取一个元素
stN{b|h|w|d}  // 存储 N 个向量到交错地址
```

N 可以是 2, 3, 4，表示通道数。

### ld3b 指令

`ld3b {z0.b, z1.b, z2.b}, pg/z, [base]`

加载 3 个向量的交错数据：
- 从 base 地址读取
- 每 3 字节分别存入 z0, z1, z2
- 使用 pg predicate 控制活跃 lane

```
内存布局:
[RGB][RGB][RGB][RGB]...

加载后:
z0.b = [R0][R1][R2][R3]...
z1.b = [G0][G1][G2][G3]...
z2.b = [B0][B1][B2][B3]...
```

## 与 NEON 的区别

### NEON 交错加载

```cpp
// 固定处理 16 像素 (48 字节)
uint8x16x3_t rgb = vld3q_u8(src);
```

### SVE 交错加载

```cpp
// 处理 vl 个像素 (VLA)
svuint8x3_t rgb = svld3_u8(pg, src);
```

SVE 版本自动适应向量长度，无需手动计算处理数量。

## RGB/BGR 转换实现

### C 语言版本

```cpp
void rgb_to_bgr_c(const u8* src, u8* dst, usize pixels) {
    for (usize i = 0; i < pixels; ++i) {
        dst[i*3 + 0] = src[i*3 + 2];  // B
        dst[i*3 + 1] = src[i*3 + 1];  // G
        dst[i*3 + 2] = src[i*3 + 0];  // R
    }
}
```

### SVE 版本

```cpp
void rgb_to_bgr_sve(const u8* src, u8* dst, usize bytes) {
    u64 offset = 0;
    svbool_t pg = svptrue_b8();

    while (offset < bytes) {
        // Calculate predicate for this iteration
        svbool_t pg_i = svwhilelt_b8(offset, bytes);

        // Load 3 vectors (R, G, B)
        svuint8x3_t rgb = svld3_u8(pg_i, src + offset);

        // Swap R and B
        svuint8x3_t bgr = svcreate3_u8(
            svget3_u8(rgb, 2),  // B
            svget3_u8(rgb, 1),  // G
            svget3_u8(rgb, 0)   // R
        );

        // Store interleaved
        svst3_u8(pg_i, dst + offset, bgr);

        // Increment by 3 * vector length
        offset += svcntb() * 3;
    }
}
```

## 性能考虑

### Predicate 开销

SVE 每次迭代需要计算 predicate：

```cpp
svbool_t pg_i = svwhilelt_b8(offset, bytes);
```

但对于长循环，这个开销可以忽略。

### 内存带宽

交错访问可能影响缓存效率：
- 连续访问 3 字节模式对预取友好
- 大向量长度可更好地利用内存带宽

## 汇编实现

```asm
rgb_to_bgr_sve:
    mov     x3, #0              // offset
    ptrue   p0.b                // all lanes active

1:
    whilelo p1.b, x3, x2        // check bounds
    ld3b    {z4.b - z6.b}, p1/z, [x0, x3]

    mov     z1.d, z6.d          // B
    mov     z2.d, z5.d          // G
    mov     z3.d, z4.d          // R

    st3b    {z1.b - z3.b}, p1, [x1, x3]

    incb    x3, all, mul #3     // offset += 3 * VL
    whilelo p1.b, x3, x2
    b.any   1b

    ret
```