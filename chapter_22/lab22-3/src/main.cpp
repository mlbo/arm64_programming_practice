// lab22-3: Auto-vectorization vs Manual NEON Test

#include <time.h>

// Import types from namespace
using arm64lab::neon::u8;
using arm64lab::neon::usize;
using arm64lab::neon::u32;

constexpr int ARRAY_SIZE = 1024 * 1024;
constexpr int IMAGE_PIXELS = 4096 * 2160;

int main() {
    struct timespec ts_start, ts_end;
    unsigned long time_scalar, time_neon;

    // === Test 1: Array Addition ===
    printf("=== Array Addition Test ===\n");

    float* a = static_cast<float*>(malloc(ARRAY_SIZE * sizeof(float)));
    float* b = static_cast<float*>(malloc(ARRAY_SIZE * sizeof(float)));
    float* c_scalar = static_cast<float*>(malloc(ARRAY_SIZE * sizeof(float)));
    float* c_neon = static_cast<float*>(malloc(ARRAY_SIZE * sizeof(float)));

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        a[i] = static_cast<float>(rand()) / RAND_MAX;
        b[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    // Scalar version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    arm64lab::neon::add_scalar(a, b, c_scalar, ARRAY_SIZE);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_scalar = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                  (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("Scalar version: %lu us\n", time_scalar);

    // NEON version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    arm64lab::neon::add_neon(a, b, c_neon, ARRAY_SIZE);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_neon = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("NEON version: %lu us\n", time_neon);

    // Verify
    bool correct = true;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        if (c_scalar[i] != c_neon[i]) {
            correct = false;
            break;
        }
    }
    printf("Results match: %s\n", correct ? "yes" : "no");
    printf("Speedup: %.2fx\n\n", static_cast<double>(time_scalar) / time_neon);

    free(a);
    free(b);
    free(c_scalar);
    free(c_neon);

    // === Test 2: RGB/BGR Conversion ===
    printf("=== RGB/BGR Conversion Test ===\n");

    usize pixel_size = IMAGE_PIXELS * 3;
    u8* rgb_src = static_cast<u8*>(malloc(pixel_size));
    u8* bgr_scalar = static_cast<u8*>(malloc(pixel_size));
    u8* bgr_neon = static_cast<u8*>(malloc(pixel_size));

    for (usize i = 0; i < pixel_size; ++i) {
        rgb_src[i] = static_cast<u8>(rand() & 0xFF);
    }

    // Scalar version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    arm64lab::neon::rgb_to_bgr_scalar(rgb_src, bgr_scalar, IMAGE_PIXELS);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_scalar = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                  (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("Scalar version: %lu us\n", time_scalar);

    // NEON version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    arm64lab::neon::rgb_to_bgr_neon(rgb_src, bgr_neon, IMAGE_PIXELS);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_neon = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("NEON version: %lu us\n", time_neon);

    // Verify
    correct = (memcmp(bgr_scalar, bgr_neon, pixel_size) == 0);
    printf("Results match: %s\n", correct ? "yes" : "no");
    printf("Speedup: %.2fx\n\n", static_cast<double>(time_scalar) / time_neon);

    free(rgb_src);
    free(bgr_scalar);
    free(bgr_neon);

    // === Test 3: Reduction ===
    printf("=== Reduction Test ===\n");

    a = static_cast<float*>(malloc(ARRAY_SIZE * sizeof(float)));
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        a[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    float sum_scalar_val, sum_neon_val;

    // Scalar version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    sum_scalar_val = arm64lab::neon::sum_scalar(a, ARRAY_SIZE);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_scalar = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                  (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("Scalar version: %lu us, sum = %f\n", time_scalar, sum_scalar_val);

    // NEON version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    sum_neon_val = arm64lab::neon::sum_neon(a, ARRAY_SIZE);
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_neon = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("NEON version: %lu us, sum = %f\n", time_neon, sum_neon_val);

    printf("Speedup: %.2fx\n", static_cast<double>(time_scalar) / time_neon);

    free(a);

    return 0;
}