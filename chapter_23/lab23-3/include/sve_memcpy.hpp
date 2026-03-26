// lab23-3: SVE memcpy Utilities

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab::sve {

using u8 = uint8_t;
using u64 = uint64_t;
using usize = size_t;

// SVE memcpy using 1-byte granularity (assembly)
extern "C" void sve_memcpy_1b(void* dst, const void* src, usize n);

// SVE memcpy using 4-byte granularity (assembly)
extern "C" void sve_memcpy_4b(void* dst, const void* src, usize n);

// SVE memcpy using intrinsics
void sve_memcpy_intr(void* dst, const void* src, usize n);

} // namespace arm64lab::sve