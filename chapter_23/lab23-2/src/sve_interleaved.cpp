// lab23-2: SVE Interleaved Load/Store Implementation

#include "sve_interleaved.hpp"
#include <arm_sve.h>

namespace arm64lab::sve {

void rgb_to_bgr_scalar(const u8* src, u8* dst, usize pixels) {
    for (usize i = 0; i < pixels; ++i) {
        dst[i * 3 + 0] = src[i * 3 + 2];  // B
        dst[i * 3 + 1] = src[i * 3 + 1];  // G
        dst[i * 3 + 2] = src[i * 3 + 0];  // R
    }
}

void rgb_to_bgr_sve_intr(const u8* src, u8* dst, usize bytes) {
    u64 offset = 0;

    while (offset < bytes) {
        // Predicate for remaining elements
        svbool_t pg = svwhilelt_b8(offset, bytes);

        // Load interleaved RGB (3 vectors)
        svuint8x3_t rgb = svld3_u8(pg, src + offset);

        // Create BGR by swapping R and B channels
        svuint8x3_t bgr = svcreate3_u8(
            svget3_u8(rgb, 2),  // B from original R position
            svget3_u8(rgb, 1),  // G unchanged
            svget3_u8(rgb, 0)   // R from original B position
        );

        // Store interleaved BGR
        svst3_u8(pg, dst + offset, bgr);

        // Increment offset by 3 * vector length in bytes
        offset += svcntb() * 3;
    }
}

void rgb_to_bgr_sve_asm(const u8* src, u8* dst, usize bytes) {
    asm volatile (
        "mov     x3, #0\n"
        "whilelo p0.b, x3, %2\n"

        "1:\n"
        "ld3b    {z4.b - z6.b}, p0/z, [%0, x3]\n"
        "mov     z1.d, z6.d\n"
        "mov     z2.d, z5.d\n"
        "mov     z3.d, z4.d\n"
        "st3b    {z1.b - z3.b}, p0, [%1, x3]\n"
        "incb    x3, all, mul #3\n"
        "whilelo p0.b, x3, %2\n"
        "b.any   1b\n"
        :
        : "r"(src), "r"(dst), "r"(bytes)
        : "memory", "x3", "p0",
          "z1", "z2", "z3", "z4", "z5", "z6"
    );
}

} // namespace arm64lab::sve