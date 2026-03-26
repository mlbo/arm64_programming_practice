// chapter_16/lab16-2/include/cache_aligned.hpp
// 缓存行对齐工具
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab {

// 缓存行大小 (通常为 64 字节)
constexpr size_t CACHE_LINE_SIZE = 64;

// 缓存行对齐包装器
template<typename T>
struct alignas(CACHE_LINE_SIZE) CacheLineAligned {
    T value;

    CacheLineAligned() : value{} {}
    explicit CacheLineAligned(T v) : value(v) {}

    operator T() const { return value; }
    auto operator=(T v) -> CacheLineAligned& {
        value = v;
        return *this;
    }
};

// 带填充的结构体
template<typename T>
struct Padded {
    T value;
    char padding_[CACHE_LINE_SIZE - sizeof(T)];

    Padded() : value{} {}
    explicit Padded(T v) : value(v) {}

    operator T() const { return value; }
    auto operator=(T v) -> Padded& {
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