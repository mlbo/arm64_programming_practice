// lab22-2: NEON Matrix Multiplication Test

#include <cstdio>
#include <ctime>
#include "neon_matrix.hpp"

constexpr int LOOP = 10000;

int main() {
    struct timespec time_start, time_end;
    unsigned long time_c, time_neon, time_asm;

    float A[16];
    float B[16];
    float C_scalar[16];
    float C_neon[16];
    float C_asm[16];

    // Initialize matrices with random values
    arm64lab::neon::matrix_init_random(A, 16);
    arm64lab::neon::matrix_init_random(B, 16);
    arm64lab::neon::matrix_init_value(C_scalar, 4, 4, 0);

    printf("Matrix A:\n");
    arm64lab::neon::matrix_print(A, 4, 4);

    printf("Matrix B:\n");
    arm64lab::neon::matrix_print(B, 4, 4);

    // Test scalar version
    clock_gettime(CLOCK_REALTIME, &time_start);
    for (int i = 0; i < LOOP; ++i) {
        arm64lab::neon::matrix_mul_4x4_scalar(A, B, C_scalar);
    }
    clock_gettime(CLOCK_REALTIME, &time_end);
    time_c = (time_end.tv_sec - time_start.tv_sec) * 1000000 +
             (time_end.tv_nsec - time_start.tv_nsec) / 1000;
    printf("C scalar version: %lu us\n", time_c);
    arm64lab::neon::matrix_print(C_scalar, 4, 4);

    // Test NEON intrinsics version
    clock_gettime(CLOCK_REALTIME, &time_start);
    for (int i = 0; i < LOOP; ++i) {
        arm64lab::neon::matrix_mul_4x4_neon(A, B, C_neon);
    }
    clock_gettime(CLOCK_REALTIME, &time_end);
    time_neon = (time_end.tv_sec - time_start.tv_sec) * 1000000 +
                (time_end.tv_nsec - time_start.tv_nsec) / 1000;
    printf("NEON intrinsics version: %lu us\n", time_neon);
    arm64lab::neon::matrix_print(C_neon, 4, 4);

    // Test NEON assembly version
    clock_gettime(CLOCK_REALTIME, &time_start);
    for (int i = 0; i < LOOP; ++i) {
        arm64lab::neon::matrix_mul_4x4_asm(A, B, C_asm);
    }
    clock_gettime(CLOCK_REALTIME, &time_end);
    time_asm = (time_end.tv_sec - time_start.tv_sec) * 1000000 +
               (time_end.tv_nsec - time_start.tv_nsec) / 1000;
    printf("NEON assembly version: %lu us\n", time_asm);
    arm64lab::neon::matrix_print(C_asm, 4, 4);

    // Verify correctness
    bool neon_ok = arm64lab::neon::matrix_compare(C_scalar, C_neon, 4, 4);
    printf("NEON equal to scalar: %s\n", neon_ok ? "yes" : "no");

    bool asm_ok = arm64lab::neon::matrix_compare(C_scalar, C_asm, 4, 4);
    printf("ASM equal to scalar: %s\n", asm_ok ? "yes" : "no");

    printf("================================\n");
    printf("ASM faster than scalar: %.2fx\n",
           static_cast<double>(time_c) / time_asm);
    printf("ASM faster than NEON intrinsics: %.2fx\n",
           static_cast<double>(time_neon) / time_asm);

    return 0;
}