// shared/baremetal/new.hpp
// ARM64 裸机编程 - Placement New 支持

#pragma once

#include <stddef.h>

// Placement new 实现 (裸机环境不需要链接标准库)
inline void* operator new(size_t, void* p) noexcept {
    return p;
}

inline void* operator new[](size_t, void* p) noexcept {
    return p;
}

// Placement delete (对应 placement new)
inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}

namespace benos {

// 便捷函数
template<typename T>
auto placement_new(void* addr) -> T* {
    return new (addr) T;
}

template<typename T>
auto placement_delete(T* p) -> void {
    if (p) {
        p->~T();
    }
}

} // namespace benos