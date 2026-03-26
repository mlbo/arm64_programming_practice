// lab23-2: SVE Interleaved Load/Store Test

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/auxv.h>
#include <asm/hwcap.h>
#include "sve_interleaved.hpp"

constexpr size_t IMAGE_PIXELS = 4096 * 2160;

int main() {
    printf("=== SVE Interleaved Load/Store Test ===\n\n");

    // Check SVE support
    if (!(getauxval(AT_HWCAP) & HWCAP_SVE)) {
        printf("SVE not supported on this system\n");
        return 1;
    }

    size_t pixel_bytes = IMAGE_PIXELS * 3;

    // Allocate buffers
    arm64lab::sve::u8* rgb_src = static_cast<arm64lab::sve::u8*>(malloc(pixel_bytes));
    arm64lab::sve::u8* bgr_scalar = static_cast<arm64lab::sve::u8*>(malloc(pixel_bytes));
    arm64lab::sve::u8* bgr_sve_intr = static_cast<arm64lab::sve::u8*>(malloc(pixel_bytes));
    arm64lab::sve::u8* bgr_sve_asm = static_cast<arm64lab::sve::u8*>(malloc(pixel_bytes));

    if (!rgb_src || !bgr_scalar || !bgr_sve_intr || !bgr_sve_asm) {
        printf("Failed to allocate buffers\n");
        return 1;
    }

    // Initialize source with random data
    for (size_t i = 0; i < pixel_bytes; ++i) {
        rgb_src[i] = static_cast<arm64lab::sve::u8>(rand() & 0xFF);
    }

    printf("Image size: %zu pixels\n", IMAGE_PIXELS);
    printf("Total bytes: %zu\n\n", pixel_bytes);

    struct timespec ts_start, ts_end;
    unsigned long time_scalar, time_intr, time_asm;

    // Scalar version
    memset(bgr_scalar, 0, pixel_bytes);
    clock_gettime(CLOCK_REALTIME, &ts_start);
    arm64lab::sve::rgb_to_bgr_scalar(rgb_src, bgr_scalar, IMAGE_PIXELS);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_scalar = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                  (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("Scalar version: %lu us\n", time_scalar);

    // SVE intrinsics version
    memset(bgr_sve_intr, 0, pixel_bytes);
    clock_gettime(CLOCK_REALTIME, &ts_start);
    arm64lab::sve::rgb_to_bgr_sve_intr(rgb_src, bgr_sve_intr, pixel_bytes);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_intr = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("SVE intrinsics: %lu us\n", time_intr);

    // SVE assembly version
    memset(bgr_sve_asm, 0, pixel_bytes);
    clock_gettime(CLOCK_REALTIME, &ts_start);
    arm64lab::sve::rgb_to_bgr_sve_asm(rgb_src, bgr_sve_asm, pixel_bytes);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_asm = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
               (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("SVE assembly: %lu us\n", time_asm);

    printf("\n=== Verification ===\n");

    bool intr_match = (memcmp(bgr_scalar, bgr_sve_intr, pixel_bytes) == 0);
    bool asm_match = (memcmp(bgr_scalar, bgr_sve_asm, pixel_bytes) == 0);

    printf("SVE intrinsics match scalar: %s\n", intr_match ? "PASSED" : "FAILED");
    printf("SVE assembly match scalar: %s\n", asm_match ? "PASSED" : "FAILED");

    printf("\n=== Speedup ===\n");
    printf("SVE intrinsics vs scalar: %.2fx\n",
           static_cast<double>(time_scalar) / time_intr);
    printf("SVE assembly vs scalar: %.2fx\n",
           static_cast<double>(time_scalar) / time_asm);

    // Cleanup
    free(rgb_src);
    free(bgr_scalar);
    free(bgr_sve_intr);
    free(bgr_sve_asm);

    return 0;
}