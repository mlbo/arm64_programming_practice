// lab22-3: Auto-vectorization vs Manual NEON Implementation

#include "auto_vectorization.hpp"
#include <arm_neon.h>

namespace arm64lab::neon {

// === Array Addition ===

void add_scalar(const float* a, const float* b, float* c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

void add_neon(const float* a, const float* b, float* c, int n) {
    int i = 0;

    // Process 4 elements at a time
    for (; i + 3 < n; i += 4) {
        float32x4_t va = vld1q_f32(a + i);
        float32x4_t vb = vld1q_f32(b + i);
        float32x4_t vc = vaddq_f32(va, vb);
        vst1q_f32(c + i, vc);
    }

    // Handle remaining elements
    for (; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

// === RGB/BGR Conversion ===

void rgb_to_bgr_scalar(const u8* src, u8* dst, usize pixels) {
    for (usize i = 0; i < pixels; ++i) {
        dst[i * 3 + 0] = src[i * 3 + 2];
        dst[i * 3 + 1] = src[i * 3 + 1];
        dst[i * 3 + 2] = src[i * 3 + 0];
    }
}

void rgb_to_bgr_neon(const u8* src, u8* dst, usize pixels) {
    usize i = 0;

    for (; i + 15 < pixels; i += 16) {
        uint8x16x3_t rgb = vld3q_u8(src + i * 3);

        uint8x16x3_t bgr;
        bgr.val[0] = rgb.val[2];
        bgr.val[1] = rgb.val[1];
        bgr.val[2] = rgb.val[0];

        vst3q_u8(dst + i * 3, bgr);
    }

    for (; i < pixels; ++i) {
        dst[i * 3 + 0] = src[i * 3 + 2];
        dst[i * 3 + 1] = src[i * 3 + 1];
        dst[i * 3 + 2] = src[i * 3 + 0];
    }
}

// === Scaling ===

void scale_no_restrict(float* a, float* b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] * 2.0f;
    }
}

void scale_with_restrict(float* __restrict__ a,
                          float* __restrict__ b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] * 2.0f;
    }
}

// === Reduction ===

float sum_scalar(const float* a, int n) {
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
}

float sum_neon(const float* a, int n) {
    float32x4_t vsum = vdupq_n_f32(0.0f);

    int i = 0;
    for (; i + 3 < n; i += 4) {
        float32x4_t v = vld1q_f32(a + i);
        vsum = vaddq_f32(vsum, v);
    }

    // Horizontal sum
    float sum = vaddvq_f32(vsum);

    // Handle remaining elements
    for (; i < n; ++i) {
        sum += a[i];
    }

    return sum;
}

} // namespace arm64lab::neon