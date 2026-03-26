# 代码说明

## 核心函数解析

### 1. 1 字节粒度 memcpy (汇编)

```asm
sve_memcpy_1b:
    mov     x3, #0
    whilelt p0.b, x3, x2

1:
    ld1b    z0.b, p0/z, [x1, x3]   // 加载字节向量
    st1b    z0.b, p0, [x0, x3]     // 存储字节向量
    incb    x3                     // x3 += 向量长度(字节)
    whilelt p0.b, x3, x2           // 更新 predicate
    b.any   1b                     // 有激活 lane 则继续

    ret
```

### 2. 4 字节粒度 memcpy (汇编)

```asm
sve_memcpy_4b:
    lsr     x2, x2, #2             // n / 4 (元素数)
    mov     x3, #0
    whilelt p0.s, x3, x2           // word 粒度 predicate

1:
    ld1w    z0.s, p0/z, [x1, x3, lsl 2]  // 加载 word 向量
    st1w    z0.s, p0, [x0, x3, lsl 2]    // 存储 word 向量
    incw    x3                     // x3 += 向量长度(word)
    whilelt p0.s, x3, x2
    b.any   1b

    ret
```

### 3. SVE Intrinsics 版本

```cpp
void sve_memcpy_intr(void* dst, const void* src, size_t n) {
    u8* d = static_cast<u8*>(dst);
    const u8* s = static_cast<const u8*>(src);
    u64 i = 0;

    while (i < n) {
        svbool_t pg = svwhilelt_b8(i, n);
        svuint8_t data = svld1_u8(pg, s + i);
        svst1_u8(pg, d + i, data);
        i += svcntb();  // Increment by vector length
    }
}
```

## 指令详解

### whilelt

```asm
whilelt p0.b, x0, x1
```

设置 predicate：对于每个 byte lane i，如果 `x0 + i < x1`，则该 lane 激活。

### incb / incw

```asm
incb    x0          // x0 += 向量长度 (字节)
incw    x0          // x0 += 向量长度 (word)
incw    x0, all, mul #4  // x0 += 4 * 向量长度 (word)
```

### ld1b / st1b

```asm
ld1b    z0.b, p0/z, [x1, x3]   // 加载字节
st1b    z0.b, p0, [x0, x3]     // 存储字节
```

`/z` 后缀表示非激活 lane 清零。

## 性能测试

```cpp
// 测试不同大小
size_t sizes[] = {256, 1024, 4096, 65536, 1048576};

for (auto size : sizes) {
    auto start = get_time();
    for (int i = 0; i < 1000; ++i) {
        sve_memcpy_1b(dst, src, size);
    }
    auto end = get_time();

    double gb_per_sec = (size * 1000.0) / (end - start) / 1e9;
    printf("Size %zu: %.2f GB/s\n", size, gb_per_sec);
}
```