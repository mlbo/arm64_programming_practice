// mm.hpp - Memory management constants and functions
#pragma once

#include "types.hpp"

#ifndef __ASSEMBLY__

namespace arm64lab {

// Assembly function declaration
extern "C" void memzero(uptr src, usize n);

} // namespace arm64lab

#endif // __ASSEMBLY__
