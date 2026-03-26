# SVE memcpy 知识

## memcpy 的 SIMD 实现

memcpy 是最常用的内存操作之一，SIMD 可以显著提升其性能。

### 传统 memcpy

```cpp
void memcpy_c(void* dst, const void* src, size_t n) {
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
}
```

### SVE memcpy

```cpp
void memcpy_sve(void* dst, const void* src, size_t n) {
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;
    u64 i = 0;

    while (i < n) {
        svbool_t pg = svwhilelt_b8(i, n);
        svuint8_t data = svld1_u8(pg, s + i);
        svst1_u8(pg, d + i, data);
        i += svcntb();
    }
}
```

## 不同粒度的实现

### 1 字节粒度

```asm
memcpy_sve_1b:
    mov     x3, #0
    whilelt p0.b, x3, x2

1:
    ld1b    z0.b, p0/z, [x1, x3]
    st1b    z0.b, p0, [x0, x3]
    incb    x3
    whilelt p0.b, x3, x2
    b.any   1b

    ret
```

### 4 字节粒度

对于 4 字节对齐的数据，可以使用字粒度操作：

```asm
memcpy_sve_4b:
    lsr     x2, x2, #2      // n / 4
    mov     x3, #0
    whilelt p0.s, x3, x2

1:
    ld1w    z0.s, p0/z, [x1, x3, lsl 2]
    st1w    z0.s, p0, [x0, x3, lsl 2]
    incw    x3
    whilelt p0.s, x3, x2
    b.any   1b

    ret
```

## 性能考虑

### 对齐

对齐访问通常更快：
- 16 字节对齐适合 128-bit 向量
- 64 字节对齐适合缓存行

### 预取

对于大数据拷贝，预取可以提升性能：

```cpp
// 软件预取
__builtin_prefetch(src + i + 1024);
```

### 循环展开

手动展开可以减少循环开销：

```cpp
while (i + 4 * svcntb() < n) {
    svuint8_t d0 = svld1_u8(pg, s + i);
    svuint8_t d1 = svld1_u8(pg, s + i + svcntb());
    svuint8_t d2 = svld1_u8(pg, s + i + 2*svcntb());
    svuint8_t d3 = svld1_u8(pg, s + i + 3*svcntb());

    svst1_u8(pg, d + i, d0);
    svst1_u8(pg, d + i + svcntb(), d1);
    svst1_u8(pg, d + i + 2*svcntb(), d2);
    svst1_u8(pg, d + i + 3*svcntb(), d3);

    i += 4 * svcntb();
}
```

## 与标准库 memcpy 对比

glibc 的 memcpy 通常已经高度优化，可能使用：
- NEON 指令
- 非时序存储 (non-temporal store)
- 硬件预取

SVE 实现的优势在于：
- 单一循环处理任意大小
- 代码简洁
- 自动适应向量长度