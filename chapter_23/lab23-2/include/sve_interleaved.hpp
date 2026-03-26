// lab23-2: SVE Interleaved Load/Store Utilities

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab::sve {

using u8 = uint8_t;
using u64 = uint64_t;
using usize = size_t;

// RGB to BGR conversion (scalar version)
void rgb_to_bgr_scalar(const u8* src, u8* dst, usize pixels);

// RGB to BGR conversion (SVE intrinsics version)
// Note: bytes is total byte count (pixels * 3)
void rgb_to_bgr_sve_intr(const u8* src, u8* dst, usize bytes);

// RGB to BGR conversion (SVE inline assembly version)
void rgb_to_bgr_sve_asm(const u8* src, u8* dst, usize bytes);

} // namespace arm64lab::sve