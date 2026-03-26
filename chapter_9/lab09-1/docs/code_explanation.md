# 代码说明

## 文件结构

```
src/
├── boot.S          # 启动汇编代码
├── kernel.cpp      # 内核主函数
├── console.cpp     # Console 类实现
├── uart.cpp        # UART 驱动
└── string.cpp      # 字符串工具函数

include/
├── console.hpp     # Console 类声明
├── uart.hpp        # UART 驱动接口
└── string.hpp      # 字符串函数声明
```

## 核心代码解析

### 1. Console 类设计 (console.hpp)

```cpp
namespace benos {

class Console {
public:
    static auto instance() -> Console& {
        static Console inst;
        return inst;
    }

    void init();
    auto printf(const char* fmt, ...) -> int;
    void putchar(char c);
    void puts(const char* str);

private:
    Console() = default;

    auto vprintf(const char* fmt, va_list args) -> int;
    auto print_number(u64 num, int base, int width, int flags) -> int;
};

Console& g_console = Console::instance();

}  // namespace benos
```

**设计要点：**
- 使用 Meyer's Singleton 模式（C++11 保证线程安全）
- 私有构造函数防止外部实例化
- 返回引用而非指针，避免空指针问题

### 2. printf 实现 (console.cpp)

```cpp
auto Console::printf(const char* fmt, ...) -> int {
    va_list args;
    va_start(args, fmt);
    int ret = vprintf(fmt, args);
    va_end(args);
    return ret;
}

auto Console::vprintf(const char* fmt, va_list args) -> int {
    int count = 0;

    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            putchar(*fmt);
            ++count;
            continue;
        }

        // 解析格式说明符
        ++fmt;  // 跳过 '%'
        // ... 格式解析逻辑
    }

    return count;
}
```

### 3. 数字格式化

```cpp
auto Console::print_number(u64 num, int base, int width, int flags) -> int {
    char buf[32];
    const char* digits = (flags & SMALL) ? "0123456789abcdef" : "0123456789ABCDEF";

    int i = 0;
    if (num == 0) {
        buf[i++] = '0';
    } else {
        while (num > 0) {
            buf[i++] = digits[num % base];
            num /= base;
        }
    }

    // 反转并输出
    while (i > 0) {
        putchar(buf[--i]);
    }

    return i;
}
```

### 4. 启动代码 (boot.S)

```asm
.section ".text.boot"
.global _start

_start:
    // 设置栈指针
    ldr x0, =_stack_top
    mov sp, x0

    // 清空 BSS 段
    ldr x0, =_bss
    ldr x1, =_ebss
    mov x2, #0
clear_bss:
    cmp x0, x1
    b.ge 2f
    str x2, [x0], #8
    b clear_bss
2:

    // 跳转到 C++ 入口
    bl kernel_main

    // 死循环
halt:
    wfi
    b halt
```

### 5. 链接脚本 (linker.ld)

```ld
ENTRY(_start)

SECTIONS
{
    . = 0x80000;

    .text.boot : { *(.text.boot) }
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : {
        _bss = .;
        *(.bss)
        _ebss = .;
    }

    _stack_top = . + 0x10000;
}
```

## 编译流程

1. **预处理**: 处理 `#include`, `#define` 等
2. **编译**: C++ -> 汇编
3. **汇编**: 汇编 -> 目标文件
4. **链接**: 目标文件 -> 可执行文件

```bash
aarch64-linux-gnu-g++ -c kernel.cpp -o kernel.o
aarch64-linux-gnu-gcc -c boot.S -o boot.o
aarch64-linux-gnu-ld -T linker.ld -o benos.elf boot.o kernel.o ...
aarch64-linux-gnu-objcopy -O binary benos.elf benos.bin
```