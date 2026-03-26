// chapter_16/lab16-1/include/cache_aligned.hpp
// 缓存行对齐工具
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab {

// 缓存行大小 (通常为 64 字节)
constexpr size_t CACHE_LINE_SIZE = 64;

// 缓存行对齐包装器
// 用于避免 false sharing
template<typename T>
struct alignas(CACHE_LINE_SIZE) CacheLineAligned {
    T value;

    CacheLineAligned() : value{} {}
    explicit CacheLineAligned(T v) : value(v) {}

    // 隐式转换
    operator T() const { return value; }
    auto operator=(T v) -> CacheLineAligned& {
        value = v;
        return *this;
    }
};

// 无符号整数类型
using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using usize = size_t;

// 有符号整数类型
using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;

} // namespace arm64lab