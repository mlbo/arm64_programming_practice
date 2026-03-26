// lab22-1: NEON RGB/BGR Conversion Test
// Compare scalar vs NEON vs ASM performance

#include "neon_utils.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Import types from namespace
using arm64lab::neon::u8;
using arm64lab::neon::usize;

// Image size: 4K resolution
constexpr usize IMAGE_SIZE = 4096 * 2160 * 10;
constexpr usize PIXEL_SIZE = IMAGE_SIZE * 3;

int main(int argc, char* argv[]) {
    struct timespec time_start, time_end;
    unsigned long time_c, time_neon, time_asm;

    // Allocate source buffer
    u8* rgb24_src = static_cast<u8*>(malloc(PIXEL_SIZE));
    if (!rgb24_src) {
        fprintf(stderr, "Failed to allocate source buffer\n");
        return 1;
    }
    memset(rgb24_src, 0, PIXEL_SIZE);

    // Allocate destination buffers
    u8* bgr24_c = static_cast<u8*>(malloc(PIXEL_SIZE));
    u8* bgr24_neon = static_cast<u8*>(malloc(PIXEL_SIZE));
    u8* bgr24_asm = static_cast<u8*>(malloc(PIXEL_SIZE));

    if (!bgr24_c || !bgr24_neon || !bgr24_asm) {
        fprintf(stderr, "Failed to allocate destination buffers\n");
        free(rgb24_src);
        return 1;
    }
    memset(bgr24_c, 0, PIXEL_SIZE);
    memset(bgr24_neon, 0, PIXEL_SIZE);
    memset(bgr24_asm, 0, PIXEL_SIZE);

    // Initialize source with random data
    for (usize i = 0; i < PIXEL_SIZE; ++i) {
        rgb24_src[i] = static_cast<u8>(rand() & 0xFF);
    }

    // Test scalar C version
    clock_gettime(CLOCK_REALTIME, &time_start);
    arm64lab::neon::rgb_to_bgr_scalar(rgb24_src, bgr24_c, IMAGE_SIZE);
    clock_gettime(CLOCK_REALTIME, &time_end);
    time_c = (time_end.tv_sec - time_start.tv_sec) * 1000 +
             (time_end.tv_nsec - time_start.tv_nsec) / 1000000;
    printf("C version spent time: %lu ms\n", time_c);

    // Test NEON intrinsics version
    clock_gettime(CLOCK_REALTIME, &time_start);
    arm64lab::neon::rgb_to_bgr_neon(rgb24_src, bgr24_neon, IMAGE_SIZE);
    clock_gettime(CLOCK_REALTIME, &time_end);
    time_neon = (time_end.tv_sec - time_start.tv_sec) * 1000 +
                (time_end.tv_nsec - time_start.tv_nsec) / 1000000;
    printf("NEON intrinsics spent time: %lu ms\n", time_neon);

    // Test NEON assembly version
    clock_gettime(CLOCK_REALTIME, &time_start);
    arm64lab::neon::rgb_to_bgr_asm(rgb24_src, bgr24_asm, IMAGE_SIZE);
    clock_gettime(CLOCK_REALTIME, &time_end);
    time_asm = (time_end.tv_sec - time_start.tv_sec) * 1000 +
               (time_end.tv_nsec - time_start.tv_nsec) / 1000000;
    printf("ASM version spent time: %lu ms\n", time_asm);

    // Verify results
    if (memcmp(bgr24_c, bgr24_neon, PIXEL_SIZE) != 0 ||
        memcmp(bgr24_c, bgr24_asm, PIXEL_SIZE) != 0) {
        printf("Error: BGR data mismatch!\n");
    } else {
        printf("BGR result (%zu bytes) is identical\n", PIXEL_SIZE);
    }

    // Print speedup
    printf("ASM faster than C: %.2fx\n", static_cast<double>(time_c) / time_asm);
    printf("ASM faster than NEON: %.2fx\n", static_cast<double>(time_neon) / time_asm);

    // Cleanup
    free(rgb24_src);
    free(bgr24_c);
    free(bgr24_neon);
    free(bgr24_asm);

    return 0;
}