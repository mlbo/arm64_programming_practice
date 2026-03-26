# 代码说明

## 系统寄存器宏实现

```cpp
// 读取系统寄存器
#define read_sysreg(reg) ({ \
    u64 _val; \
    asm volatile("mrs %0, " #reg : "=r" (_val)); \
    _val; \
})

// 写入系统寄存器
#define write_sysreg(val, reg) ({ \
    u64 _val = (u64)(val); \
    asm volatile("msr " #reg ", %x0" :: "rZ" (_val)); \
})
```

## asm goto 示例

```cpp
static int test_asm_goto(int a) {
    asm goto (
        "cmp %w0, #1\n"
        "b.eq %l[label]\n"
        :
        : "r" (a)
        : "memory"
        : label
    );
    return 0;

label:
    printk("Jumped to label, a=%d\n", a);
    return 1;
}
```

## 命名操作数

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