// lab23-3: SVE memcpy Test

#include <asm/hwcap.h>

// Import types from namespace
using arm64lab::sve::u8;
using arm64lab::sve::usize;
using arm64lab::sve::u32;

constexpr usize SIZE = 256;

int main() {
    printf("=== SVE memcpy Test ===\n\n");

    if (!(getauxval(AT_HWCAP) & HWCAP_SVE)) {
        printf("SVE not supported\n");
        return 1;
    }

    static u8 a[SIZE];
    static u8 b[SIZE];

    // Initialize source
    for (usize i = 0; i < SIZE; ++i) {
        a[i] = static_cast<u8>(i);
    }
    memset(b, 0, SIZE);

    // Test 1-byte granularity
    printf("Testing 1-byte granularity memcpy...\n");
    arm64lab::sve::sve_memcpy_1b(b, a, SIZE);

    bool success = true;
    for (usize i = 0; i < SIZE; ++i) {
        if (a[i] != b[i]) {
            printf("Mismatch at %zu: a=%u, b=%u\n", i, a[i], b[i]);
            success = false;
        }
    }
    printf("1-byte granularity test: %s\n\n", success ? "PASSED" : "FAILED");

    // Test 4-byte granularity
    memset(b, 0, SIZE);
    printf("Testing 4-byte granularity memcpy...\n");
    arm64lab::sve::sve_memcpy_4b(b, a, SIZE);

    success = true;
    for (usize i = 0; i < SIZE; ++i) {
        if (a[i] != b[i]) {
            printf("Mismatch at %zu: a=%u, b=%u\n", i, a[i], b[i]);
            success = false;
        }
    }
    printf("4-byte granularity test: %s\n\n", success ? "PASSED" : "FAILED");

    // Test intrinsics version
    memset(b, 0, SIZE);
    printf("Testing intrinsics memcpy...\n");
    arm64lab::sve::sve_memcpy_intr(b, a, SIZE);

    success = true;
    for (usize i = 0; i < SIZE; ++i) {
        if (a[i] != b[i]) {
            printf("Mismatch at %zu: a=%u, b=%u\n", i, a[i], b[i]);
            success = false;
        }
    }
    printf("Intrinsics test: %s\n\n", success ? "PASSED" : "FAILED");

    // Performance test
    printf("=== Performance Test ===\n");
    constexpr usize PERF_SIZE = 64 * 1024 * 1024;  // 64 MB
    constexpr int ITERATIONS = 100;

    u8* src = static_cast<u8*>(malloc(PERF_SIZE));
    u8* dst = static_cast<u8*>(malloc(PERF_SIZE));

    if (!src || !dst) {
        printf("Failed to allocate buffers\n");
        return 1;
    }

    memset(src, 0xAB, PERF_SIZE);

    struct timespec ts_start, ts_end;

    // glibc memcpy
    clock_gettime(CLOCK_REALTIME, &ts_start);
    for (int i = 0; i < ITERATIONS; ++i) {
        memcpy(dst, src, PERF_SIZE);
    }
    clock_gettime(CLOCK_REALTIME, &ts_end);
    unsigned long time_glibc = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                               (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;

    // SVE memcpy
    clock_gettime(CLOCK_REALTIME, &ts_start);
    for (int i = 0; i < ITERATIONS; ++i) {
        arm64lab::sve::sve_memcpy_1b(dst, src, PERF_SIZE);
    }
    clock_gettime(CLOCK_REALTIME, &ts_end);
    unsigned long time_sve = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                             (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;

    printf("glibc memcpy: %lu us\n", time_glibc);
    printf("SVE memcpy:   %lu us\n", time_sve);
    printf("Ratio: %.2f\n", static_cast<double>(time_sve) / time_glibc);

    free(src);
    free(dst);

    return 0;
}