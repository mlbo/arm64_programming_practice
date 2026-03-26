// lab22-1: NEON RGB/BGR Conversion Implementation
// References ncnn project style

#include "neon_utils.hpp"
#include <arm_neon.h>

namespace arm64lab::neon {

void rgb_to_bgr_scalar(const u8* src, u8* dst, usize pixels) {
    for (usize i = 0; i < pixels; ++i) {
        dst[i * 3 + 0] = src[i * 3 + 2];  // B = src R
        dst[i * 3 + 1] = src[i * 3 + 1];  // G = src G
        dst[i * 3 + 2] = src[i * 3 + 0];  // R = src B
    }
}

void rgb_to_bgr_neon(const u8* src, u8* dst, usize pixels) {
    usize i = 0;

    // Process 16 pixels at a time (48 bytes)
    for (; i + 15 < pixels; i += 16) {
        // Interleaved load RGB
        uint8x16x3_t rgb = vld3q_u8(src + i * 3);

        // Swap R and B channels
        uint8x16x3_t bgr;
        bgr.val[0] = rgb.val[2];  // B
        bgr.val[1] = rgb.val[1];  // G
        bgr.val[2] = rgb.val[0];  // R

        // Interleaved store BGR
        vst3q_u8(dst + i * 3, bgr);
    }

    // Handle remaining pixels
    for (; i < pixels; ++i) {
        dst[i * 3 + 0] = src[i * 3 + 2];
        dst[i * 3 + 1] = src[i * 3 + 1];
        dst[i * 3 + 2] = src[i * 3 + 0];
    }
}

void rgb_to_bgr_asm(const u8* src, u8* dst, usize pixels) {
    usize count = pixels * 3;
    usize offset = 0;

    asm volatile (
        "1: ld3 {v0.16b, v1.16b, v2.16b}, [%[src]], #48 \n"
        "mov v3.16b, v0.16b\n"
        "mov v0.16b, v2.16b\n"
        "mov v2.16b, v3.16b\n"
        "st3 {v0.16b, v1.16b, v2.16b}, [%[dst]], #48\n"
        "add %[offset], %[offset], #48\n"
        "cmp %[offset], %[count]\n"
        "bne 1b\n"
        : [dst] "+r"(dst), [src] "+r"(src), [offset] "+r"(offset)
        : [count] "r" (count)
        : "memory", "v0", "v1", "v2", "v3"
    );
}

} // namespace arm64lab::neon