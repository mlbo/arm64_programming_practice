// include/benos/kernel/console.hpp
#pragma once

#include "benos/types.hpp"
#include <stdarg.h>

namespace benos {

class Console {
public:
    int printf(const char* fmt, ...);
    int vprintf(const char* fmt, va_list args);
    void putchar(char c);
    void puts(const char* s);

    static auto instance() -> Console&;

private:
    Console() = default;
};

extern Console& g_console;

} // namespace benos

// C compatibility
extern "C" {
    int printk(const char* fmt, ...);
    int vprintk(const char* fmt, va_list args);
}