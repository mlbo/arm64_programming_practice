// include/benos/new.hpp
#pragma once

#include "types.hpp"

// Placement new for bare metal environment
inline void* operator new(benos::usize, void* p) noexcept { return p; }
inline void* operator new[](benos::usize, void* p) noexcept { return p; }
inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}