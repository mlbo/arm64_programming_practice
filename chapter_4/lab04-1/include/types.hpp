// types.hpp - ARM64 bare-metal type definitions
// 使用编译器内置类型，不依赖标准库
#pragma once

// Constants needed by both C++ and assembly
#define PAGE_SHIFT    12
#define TABLE_SHIFT   9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE     (1ULL << PAGE_SHIFT)
#define SECTION_SIZE  (1ULL << SECTION_SHIFT)
#define LOW_MEMORY    (2 * SECTION_SIZE)

#ifndef __ASSEMBLY__

namespace arm64lab {

// 使用 GCC/Clang 内置类型定义 (freestanding 兼容)
using u8   = __UINT8_TYPE__;
using u16  = __UINT16_TYPE__;
using u32  = __UINT32_TYPE__;
using u64  = __UINT64_TYPE__;
using usize = __SIZE_TYPE__;
using uptr = __UINTPTR_TYPE__;

using i8   = __INT8_TYPE__;
using i16  = __INT16_TYPE__;
using i32  = __INT32_TYPE__;
using i64  = __INT64_TYPE__;
using isize = __PTRDIFF_TYPE__;
using iptr = __INTPTR_TYPE__;

constexpr uptr PAGE_MASK = ~(PAGE_SIZE - 1);

constexpr auto page_align_up(uptr addr) -> uptr {
    return (addr + PAGE_SIZE - 1) & PAGE_MASK;
}

} // namespace arm64lab

#endif // __ASSEMBLY__
