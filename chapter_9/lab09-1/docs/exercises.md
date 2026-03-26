# 练习题

## 基础题

### 1. 实现简单的 printk

实现一个简化版的 `printk`，只支持 `%d` 和 `%s` 格式说明符。

```cpp
// 提示：va_list 使用方法
auto simple_printk(const char* fmt, ...) -> int {
    va_list args;
    va_start(args, fmt);

    // TODO: 实现

    va_end(args);
    return 0;
}
```

### 2. 数字转字符串

不使用标准库函数，实现将整数转换为字符串：

```cpp
void int_to_str(char* buf, int value);
// 示例：int_to_str(buf, -1234) -> "-1234"
```

### 3. 十六进制输出

实现 `%x` 和 `%X` 格式说明符：

```cpp
void print_hex(u64 value, bool uppercase);
// 示例：print_hex(0x1234abcd, false) -> "1234abcd"
//       print_hex(0x1234abcd, true)  -> "1234ABCD"
```

## 进阶题

### 4. 格式化宽度

实现格式化宽度控制：

```cpp
// 示例：
printk("%10d", 123);   // 输出: "       123" (右对齐，宽度10)
printk("%-10d", 123);  // 输出: "123       " (左对齐，宽度10)
printk("%010d", 123);  // 输出: "0000000123" (零填充)
```

### 5. 精度控制

实现浮点数精度控制（提示：使用定点数表示）：

```cpp
// 示例：
printk("%.2f", 3.14159);  // 输出: "3.14"
```

### 6. 指针格式化

实现 `%p` 格式说明符：

```cpp
// 示例：
int x = 10;
printk("%p", &x);  // 输出: "0x0000007ffe1234"
```

## 挑战题

### 7. 完整的 printf 实现

参考 Linux 内核的 `vsnprintf` 实现，添加以下功能：

- 所有基本格式说明符
- 长度修饰符 (h, l, ll)
- 标志 (-, +, space, #, 0)
- 宽度和精度

### 8. 日志缓冲区

实现一个环形缓冲区用于存储日志：

```cpp
class LogBuffer {
public:
    void write(const char* data, usize len);
    auto read(char* buf, usize len) -> usize;
    void clear();

private:
    static constexpr usize SIZE = 4096;
    char buffer_[SIZE];
    usize head_ = 0;
    usize tail_ = 0;
};
```

### 9. 彩色输出

实现 ANSI 转义序列支持彩色输出：

```cpp
// 示例：
printk("\033[31mError:\033[0m something wrong\n");
// 输出红色的 "Error:" 和正常颜色的后续文本
```

## 思考题

1. 为什么在裸机环境下不能使用标准库的 `printf`？

2. ARM64 调用约定中，可变参数是如何传递的？

3. `va_list` 在 ARM64 上的实现与 x86_64 有什么不同？

4. 如何避免缓冲区溢出？在裸机环境下有哪些特殊考虑？

## 参考代码

### 练习1 参考答案

```cpp
auto simple_printk(const char* fmt, ...) -> int {
    va_list args;
    va_start(args, fmt);

    int count = 0;
    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            putchar(*fmt);
            ++count;
            continue;
        }

        ++fmt;
        switch (*fmt) {
        case 'd': {
            int val = va_arg(args, int);
            count += print_int(val);
            break;
        }
        case 's': {
            const char* s = va_arg(args, const char*);
            count += print_str(s);
            break;
        }
        default:
            putchar(*fmt);
            ++count;
        }
    }

    va_end(args);
    return count;
}
```