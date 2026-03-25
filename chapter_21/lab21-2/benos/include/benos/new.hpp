// include/benos/new.hpp
#pragma once

// Placement new/delete for bare metal (freestanding) environment
// Note: Do not include <new> - we provide our own operators

#include "types.hpp"

inline void* operator new(benos::usize, void* p) noexcept { return p; }
inline void* operator new[](benos::usize, void* p) noexcept { return p; }
inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}