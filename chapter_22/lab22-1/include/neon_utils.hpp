// lab22-1: NEON RGB/BGR Conversion Utilities
// References ncnn project style

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <arm_neon.h>

namespace arm64lab::neon {

// Type aliases
using u8 = uint8_t;
using usize = size_t;

// RGB24 to BGR24 conversion (scalar version)
void rgb_to_bgr_scalar(const u8* src, u8* dst, usize pixels);

// RGB24 to BGR24 conversion (NEON intrinsics version)
void rgb_to_bgr_neon(const u8* src, u8* dst, usize pixels);

// RGB24 to BGR24 conversion (inline assembly version)
void rgb_to_bgr_asm(const u8* src, u8* dst, usize pixels);

} // namespace arm64lab::neon