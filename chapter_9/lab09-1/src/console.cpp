// Console implementation
// Reference: Linux kernel vsprintf.c

#include "console.hpp"
#include "uart.hpp"
#include "string.hpp"

namespace benos {

// Global console reference
Console& g_console = Console::instance();

void Console::init() {
    if (initialized_) return;

    g_uart.init();
    initialized_ = true;
}

void Console::putchar(char c) {
    g_uart.send(c);
}

void Console::puts(const char* str) {
    while (*str) {
        if (*str == '\n') {
            g_uart.send('\r');
        }
        g_uart.send(*str++);
    }
}

auto Console::printf(const char* fmt, ...) -> int {
    va_list args;
    va_start(args, fmt);
    int ret = vprintf(fmt, args);
    va_end(args);
    return ret;
}

// Helper: check if character is a digit
static inline auto is_digit(char c) -> bool {
    return c >= '0' && c <= '9';
}

// Helper: scan number from string
static auto scan_number(const char** fmt) -> int {
    int num = 0;
    while (is_digit(**fmt)) {
        num = num * 10 + (**fmt - '0');
        ++(*fmt);
    }
    --(*fmt);  // Back up one character
    return num;
}

auto Console::vprintf(const char* fmt, va_list args) -> int {
    int count = 0;
    char buf[32];
    const char* str;
    int base, width, precision, flags, qualifier;
    u64 num;
    i64 snum;

    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            putchar(*fmt);
            ++count;
            continue;
        }

        // Parse flags
        flags = 0;
    repeat:
        ++fmt;
        switch (*fmt) {
        case '-': flags |= PRINTK_LEFT; goto repeat;
        case '+': flags |= PRINTK_PLUS; goto repeat;
        case ' ': flags |= PRINTK_SPACE; goto repeat;
        case '#': flags |= PRINTK_SPECIAL; goto repeat;
        case '0': flags |= PRINTK_ZEROPAD; goto repeat;
        }

        // Parse width
        width = -1;
        if (is_digit(*fmt)) {
            width = scan_number(&fmt);
        } else if (*fmt == '*') {
            width = va_arg(args, int);
            if (width < 0) {
                width = -width;
                flags |= PRINTK_LEFT;
            }
        }

        // Parse precision
        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (is_digit(*fmt)) {
                precision = scan_number(&fmt);
            } else if (*fmt == '*') {
                precision = va_arg(args, int);
            }
            if (precision < 0) precision = 0;
        }

        // Parse qualifier
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
            qualifier = *fmt;
            ++fmt;
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
                ++fmt;
            }
        }

        // Parse conversion specifier
        base = 10;
        switch (*fmt) {
        case 'c':
            if (!(flags & PRINTK_LEFT)) {
                while (--width > 0) {
                    putchar(' ');
                    ++count;
                }
            }
            putchar(static_cast<char>(va_arg(args, int)));
            ++count;
            while (--width > 0) {
                putchar(' ');
                ++count;
            }
            continue;

        case 's':
            str = va_arg(args, const char*);
            if (!str) str = "<NULL>";
            count += print_string(str, width, precision, flags);
            continue;

        case 'p':
            if (width == -1) {
                width = 2 * sizeof(void*);
                flags |= PRINTK_ZEROPAD;
            }
            count += print_number(
                reinterpret_cast<uptr>(va_arg(args, void*)),
                16, width, precision, flags);
            continue;

        case 'o':
            base = 8;
            break;

        case 'x':
            flags |= PRINTK_SMALL;
            // fall through
        case 'X':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= PRINTK_SIGN;
            // fall through
        case 'u':
            break;

        default:
            if (*fmt != '%') {
                putchar('%');
                ++count;
            }
            if (*fmt) {
                putchar(*fmt);
                ++count;
            } else {
                --fmt;
            }
            continue;
        }

        // Get number argument
        if (flags & PRINTK_SIGN) {
            switch (qualifier) {
            case 'L': snum = va_arg(args, i64); break;
            case 'l': snum = va_arg(args, long); break;
            case 'h': snum = static_cast<i16>(va_arg(args, int)); break;
            default:  snum = va_arg(args, int); break;
            }
            if (snum < 0) {
                num = static_cast<u64>(-snum);
                putchar('-');
                ++count;
            } else {
                num = static_cast<u64>(snum);
            }
        } else {
            switch (qualifier) {
            case 'L': num = va_arg(args, u64); break;
            case 'l': num = va_arg(args, unsigned long); break;
            case 'h': num = static_cast<u16>(va_arg(args, unsigned int)); break;
            default:  num = va_arg(args, unsigned int); break;
            }
        }

        count += print_number(num, base, width, precision, flags);
    }

    return count;
}

auto Console::print_number(u64 num, int base, int width, int precision, int flags) -> int {
    char buf[32];
    const char* digits = (flags & PRINTK_SMALL)
        ? "0123456789abcdef"
        : "0123456789ABCDEF";

    int i = 0;
    if (num == 0) {
        buf[i++] = '0';
    } else {
        while (num > 0) {
            buf[i++] = digits[num % base];
            num /= base;
        }
    }

    // Handle precision
    if (i > precision) precision = i;

    // Handle special prefix
    char sign = 0;
    if (flags & PRINTK_SPECIAL) {
        if (base == 8) {
            sign = '0';
        } else if (base == 16) {
            sign = 'x';
        }
    }

    width -= precision;
    int count = 0;

    // Left padding
    if (!(flags & (PRINTK_ZEROPAD | PRINTK_LEFT))) {
        while (width-- > 0) {
            putchar(' ');
            ++count;
        }
    }

    // Sign or prefix
    if (sign) {
        putchar('0');
        ++count;
        if (sign == 'x') {
            putchar(digits[33]);  // 'x' or 'X'
            ++count;
        }
    }

    // Zero padding
    if (!(flags & PRINTK_LEFT)) {
        char pad = (flags & PRINTK_ZEROPAD) ? '0' : ' ';
        while (width-- > 0) {
            putchar(pad);
            ++count;
        }
    }

    // Precision padding
    while (i < precision--) {
        putchar('0');
        ++count;
    }

    // Output digits (reversed)
    while (i-- > 0) {
        putchar(buf[i]);
        ++count;
    }

    // Right padding
    while (width-- > 0) {
        putchar(' ');
        ++count;
    }

    return count;
}

auto Console::print_string(const char* str, int width, int precision, int flags) -> int {
    int len = static_cast<int>(strlen(str));
    if (precision >= 0 && len > precision) {
        len = precision;
    }

    int count = 0;

    // Left padding
    if (!(flags & PRINTK_LEFT)) {
        while (len < width--) {
            putchar(' ');
            ++count;
        }
    }

    // Output string
    for (int i = 0; i < len; ++i) {
        putchar(str[i]);
        ++count;
    }

    // Right padding
    while (len < width--) {
        putchar(' ');
        ++count;
    }

    return count;
}

}  // namespace benos