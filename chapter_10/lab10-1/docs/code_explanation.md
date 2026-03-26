# 代码说明

## memcpy 内联汇编实现

### 基本实现

```cpp
void my_memcpy(void* dst, const void* src, usize count) {
    asm volatile (
        "1: ldr x0, [%1], #8\n"
        "str x0, [%0], #8\n"
        "subs %2, %2, #8\n"
        "b.ne 1b"
        : "+r" (dst), "+r" (src), "+r" (count)
        :
        : "x0", "memory"
    );
}
```

### 使用命名操作数

```cpp
void my_memcpy(void* dst, const void* src, usize count) {
    asm volatile (
        "1: ldr x0, [%[src]], #8\n"
        "str x0, [%[dst]], #8\n"
        "subs %[cnt], %[cnt], #8\n"
        "b.ne 1b"
        : [dst] "+r" (dst), [src] "+r" (src), [cnt] "+r" (count)
        :
        : "x0", "memory"
    );
}
```

## 使用宏简化

```cpp
#define MY_OPS(name, asm_op) \
static inline void my_asm_##name(u64 mask, void* p) { \
    u64 tmp; \
    asm volatile ( \
        "ldr %0, [%1]\n" \
        " " #asm_op " %0, %0, %2\n" \
        "str %0, [%1]\n" \
        : "+r" (tmp), "+r" (p) \
        : "r" (mask) \
        : "memory" \
    ); \
}

MY_OPS(or, orr)
MY_OPS(and, and)
MY_OPS(andnot, bic)
```

## 内联汇编注意事项

1. **使用 volatile**: 防止编译器优化掉有用的代码
2. **列出破坏项**: 避免寄存器冲突
3. **使用命名操作数**: 提高可读性
4. **避免过度使用**: 编译器优化通常更好