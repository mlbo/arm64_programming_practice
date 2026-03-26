// io.hpp - MMIO operations
#pragma once

#include "types.hpp"

namespace arm64lab {
namespace io {

// Memory barrier
inline void dmb() {
    __asm__ __volatile__ ("" : : : "memory");
}

inline void iormb() { dmb(); }
inline void iowmb() { dmb(); }

// MMIO read
inline auto readl(uptr addr) -> u32 {
    u32 v = *reinterpret_cast<volatile u32*>(addr);
    iormb();
    return v;
}

// MMIO write
inline void writel(u32 value, uptr addr) {
    iowmb();
    *reinterpret_cast<volatile u32*>(addr) = value;
}

// Simple delay loop
inline void delay(u32 n) {
    while (n--) {
        __asm__ __volatile__ ("nop");
    }
}

} // namespace io
} // namespace arm64lab