// io.hpp - MMIO operations
#pragma once

#include "types.hpp"

namespace arm64lab {
namespace io {

inline void dmb() {
    __asm__ __volatile__ ("" : : : "memory");
}

inline auto readl(uptr addr) -> u32 {
    u32 v = *reinterpret_cast<volatile u32*>(addr);
    dmb();
    return v;
}

inline void writel(u32 value, uptr addr) {
    dmb();
    *reinterpret_cast<volatile u32*>(addr) = value;
}

inline void delay(u32 n) {
    while (n--) { __asm__ __volatile__ ("nop"); }
}

} // namespace io
} // namespace arm64lab