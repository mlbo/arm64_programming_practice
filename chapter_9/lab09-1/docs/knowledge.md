# 格式化输出知识点

## 1. 可变参数 (Variadic Arguments)

### va_list 系列

C/C++ 提供了 `<stdarg.h>` 来处理可变参数：

```c
void my_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);    // 初始化 args，fmt 是最后一个固定参数
    // ... 使用 va_arg(args, type) 获取参数
    va_end(args);           // 清理
}
```

### ARM64 调用约定

ARM64 使用 AAPCS64 调用约定：
- 参数通过 x0-x7 寄存器传递
- 额外参数通过栈传递
- `va_list` 在 ARM64 上是一个结构体，包含栈指针和寄存器保存区

## 2. 格式说明符解析

### 常用格式说明符

| 说明符 | 含义 | 示例 |
|--------|------|------|
| `%d` | 有符号十进制 | `123`, `-456` |
| `%u` | 无符号十进制 | `123`, `456` |
| `%x` | 十六进制 (小写) | `1a2b` |
| `%X` | 十六进制 (大写) | `1A2B` |
| `%s` | 字符串 | `hello` |
| `%c` | 字符 | `A` |
| `%p` | 指针 | `0x0000000080001000` |

### 格式修饰符

```
%[flags][width][.precision][length]specifier

flags:     -, +, space, #, 0
width:     数字或 *
precision: .数字 或 .*
length:    h, l, ll, L
```

## 3. 数字转字符串算法

### 除法取余法

将数字转换为字符串的核心算法：

```c
void number_to_string(char* buf, unsigned long num, int base) {
    const char* digits = "0123456789abcdef";
    int i = 0;

    if (num == 0) {
        buf[i++] = '0';
    } else {
        while (num > 0) {
            buf[i++] = digits[num % base];
            num /= base;
        }
    }

    // 反转字符串
    reverse(buf, i);
}
```

### 负数处理

```c
if (num < 0) {
    *buf++ = '-';
    num = -num;  // 转为正数处理
}
```

## 4. UART 输出

### BCM2835 UART 寄存器

树莓派的 PL011 UART 寄存器：

| 偏移 | 寄存器 | 功能 |
|------|--------|------|
| 0x00 | DR     | 数据寄存器 |
| 0x18 | FR     | 标志寄存器 |
| 0x24 | IBRD   | 整数波特率除数 |
| 0x28 | FBRD   | 小数波特率除数 |
| 0x2C | LCRH   | 线控制寄存器 |
| 0x30 | CR     | 控制寄存器 |

### 发送字符

```c
void uart_send(char c) {
    // 等待发送缓冲区空
    while (UART0_FR & (1 << 5));  // TXFF 标志
    UART0_DR = c;
}
```

## 5. C++ 封装设计

### Console 类 (参考 ncnn 风格)

```cpp
namespace benos {

class Console {
public:
    // 单例模式
    static auto instance() -> Console&;

    void init();
    auto printf(const char* fmt, ...) -> int;
    void putchar(char c);
    void puts(const char* str);

private:
    Console() = default;

    auto vprintf(const char* fmt, va_list args) -> int;
    auto print_int(i64 val, int base, int width, char pad) -> int;
    auto print_uint(u64 val, int base, int width, char pad) -> int;
};

// 全局引用
extern Console& g_console;

}  // namespace benos
```

### 设计要点

1. **单例模式**: 使用静态局部变量实现线程安全的单例
2. **私有构造**: 防止外部实例化
3. **全局引用**: 提供类似 `g_console.printf()` 的简洁调用方式
4. **命名空间**: 使用 `benos` 命名空间避免冲突