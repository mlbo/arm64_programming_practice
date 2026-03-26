# 扩展内联汇编知识点

## 1. asm goto

`asm goto` 允许汇编代码跳转到 C 标签：

```cpp
int test_value(int a) {
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
    return 1;
}
```

## 2. 系统寄存器访问宏

### 读取系统寄存器

```cpp
#define read_sysreg(reg) ({ \
    u64 _val; \
    asm volatile("mrs %0, " #reg \
        : "=r" (_val)); \
    _val; \
})
```

### 写入系统寄存器

```cpp
#define write_sysreg(val, reg) ({ \
    u64 _val = (u64)(val); \
    asm volatile("msr " #reg ", %0" \
        :: "r" (_val)); \
})
```

### 使用示例

```cpp
u64 el = read_sysreg(CurrentEL);
write_sysreg(0x10000, vbar_el1);
```