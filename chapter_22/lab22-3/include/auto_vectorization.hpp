// lab22-3: Auto-vectorization vs Manual NEON

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab::neon {

using u8 = uint8_t;
using usize = size_t;

// === Array Addition ===

// Scalar version (compiler may auto-vectorize)
void add_scalar(const float* a, const float* b, float* c, int n);

// Manual NEON version
void add_neon(const float* a, const float* b, float* c, int n);

// === RGB/BGR Conversion ===

// Scalar version (hard to auto-vectorize due to interleaved data)
void rgb_to_bgr_scalar(const u8* src, u8* dst, usize pixels);

// Manual NEON with interleaved load/store
void rgb_to_bgr_neon(const u8* src, u8* dst, usize pixels);

// === Scaling ===

// Without restrict keyword
void scale_no_restrict(float* a, float* b, int n);

// With restrict keyword
void scale_with_restrict(float* __restrict__ a,
                          float* __restrict__ b, int n);

// === Reduction ===

// Scalar reduction
float sum_scalar(const float* a, int n);

// NEON reduction
float sum_neon(const float* a, int n);

} // namespace arm64lab::neon