// Lab09-1: Console class for formatted output
// Reference: ncnn coding style

#pragma once

#include <stdarg.h>
#include "../../shared/baremetal/types.hpp"

namespace benos {

// Format flags
enum : u32 {
    PRINTK_ZEROPAD = 1,    // Pad with zero
    PRINTK_SIGN     = 2,    // Unsigned/signed
    PRINTK_PLUS     = 4,    // Show plus
    PRINTK_SPACE    = 8,    // Space if plus
    PRINTK_LEFT     = 16,   // Left justified
    PRINTK_SPECIAL  = 32,   // 0x prefix
    PRINTK_SMALL    = 64,   // Lowercase hex
};

// Console class for UART output
class Console {
public:
    // Singleton access
    static auto instance() -> Console& {
        static Console inst;
        return inst;
    }

    // Initialize console
    void init();

    // Formatted print
    auto printf(const char* fmt, ...) -> int;

    // Basic output
    void putchar(char c);
    void puts(const char* str);

    // Status
    auto is_ready() const -> bool { return initialized_; }

private:
    Console() = default;

    // Internal formatting
    auto vprintf(const char* fmt, va_list args) -> int;
    auto print_number(u64 num, int base, int width, int precision, int flags) -> int;
    auto print_string(const char* str, int width, int precision, int flags) -> int;

    bool initialized_ = false;
};

// Global console reference
extern Console& g_console;

// Convenience macros
#define printk(fmt, ...) benos::g_console.printf(fmt, ##__VA_ARGS__)

}  // namespace benos