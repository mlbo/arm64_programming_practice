// lab23-5: SVE Matrix Multiplication Test
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/auxv.h>

#include <asm/hwcap.h>
#include "sve_matrix.hpp"

// Import types from namespace

constexpr int LOOP = 10000;

int main() {
    printf("=== SVE Matrix Multiplication Test ===\n\n");

    if (!(getauxval(AT_HWCAP) & HWCAP_SVE)) {
        printf("SVE not supported\n");
        return 1;
    }

    struct timespec ts_start, ts_end;
    unsigned long time_scalar, time_sve;

    float A[16];
    float B[16];
    float C_scalar[16];
    float C_sve[16];

    // Initialize matrices
    arm64lab::sve::matrix_init_random(A, 16);
    arm64lab::sve::matrix_init_random(B, 16);
    arm64lab::sve::matrix_init_value(C_scalar, 4, 4, 0);

    printf("Matrix A:\n");
    arm64lab::sve::matrix_print(A, 4, 4);

    printf("Matrix B:\n");
    arm64lab::sve::matrix_print(B, 4, 4);

    // Scalar version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    for (int i = 0; i < LOOP; ++i) {
        arm64lab::sve::matrix_mul_4x4_scalar(A, B, C_scalar);
    }
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_scalar = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
                  (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("Scalar version: %lu us\n", time_scalar);
    arm64lab::sve::matrix_print(C_scalar, 4, 4);

    // SVE version
    clock_gettime(CLOCK_REALTIME, &ts_start);
    for (int i = 0; i < LOOP; ++i) {
        arm64lab::sve::matrix_mul_4x4_sve(A, B, C_sve);
    }
    clock_gettime(CLOCK_REALTIME, &ts_end);
    time_sve = (ts_end.tv_sec - ts_start.tv_sec) * 1000000 +
               (ts_end.tv_nsec - ts_start.tv_nsec) / 1000;
    printf("SVE version: %lu us\n", time_sve);
    arm64lab::sve::matrix_print(C_sve, 4, 4);

    // Verify
    bool match = arm64lab::sve::matrix_compare(C_scalar, C_sve, 4, 4);
    printf("SVE equal to scalar: %s\n", match ? "yes" : "no");

    printf("================================\n");
    printf("SVE faster than scalar: %.2fx\n",
           static_cast<double>(time_scalar) / time_sve);

    return 0;
}