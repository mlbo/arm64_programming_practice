// memset.hpp - memset declarations
#pragma once

#include "types.hpp"

namespace arm64lab {

extern "C" void* __memset_16bytes(void* s, u64 val, u64 count);

void* memset(void* s, int c, usize count);

} // namespace arm64lab