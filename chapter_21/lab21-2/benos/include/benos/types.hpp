// include/benos/types.hpp
#pragma once

#include <stdint.h>
#include <stddef.h>

namespace benos {

using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using usize = size_t;
using uptr = uintptr_t;

using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;
using isize = ptrdiff_t;
using iptr = intptr_t;

// Memory constants
constexpr usize PAGE_SHIFT = 12;
constexpr usize PAGE_SIZE  = 1ULL << PAGE_SHIFT;
constexpr usize PAGE_MASK  = ~(PAGE_SIZE - 1);

constexpr auto page_align_up(uptr addr) -> uptr {
    return (addr + PAGE_SIZE - 1) & PAGE_MASK;
}

} // namespace benos