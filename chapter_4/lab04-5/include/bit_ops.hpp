// bit_ops.hpp - Bit field operations
#pragma once

#include "types.hpp"

namespace arm64lab {
namespace bit {

// Extract bit field (unsigned)
template<typename T>
constexpr auto extract(T val, u32 lsb, u32 width) -> T {
    return (val >> lsb) & ((T(1) << width) - 1);
}

// Insert bit field
template<typename T>
constexpr auto insert(T dst, T src, u32 lsb, u32 width) -> T {
    T mask = ((T(1) << width) - 1) << lsb;
    return (dst & ~mask) | ((src << lsb) & mask);
}

// Test single bit
template<typename T>
constexpr auto test(T val, u32 bit) -> bool {
    return (val >> bit) & 1;
}

// Set bit
template<typename T>
constexpr auto set(T val, u32 bit) -> T {
    return val | (T(1) << bit);
}

// Clear bit
template<typename T>
constexpr auto clear(T val, u32 bit) -> T {
    return val & ~(T(1) << bit);
}

} // namespace bit
} // namespace arm64lab