// lab23-1: SVE Basic Test

#include <arm_sve.h>
#include <cstdio>
#include "sve_basic.hpp"

// Import types from namespace
using arm64lab::sve::u64;

int main() {
    printf("=== SVE Basic Test ===\n\n");

    // Check SVE support and print vector info
    arm64lab::sve::print_vector_info();

    if (!arm64lab::sve::has_sve()) {
        printf("Cannot run SVE tests without hardware support\n");
        return 1;
    }

    printf("\n=== Predicate Test ===\n");

    // Get vector length in 32-bit elements
    u64 vl = arm64lab::sve::get_vector_length();
    u64 num_words = vl / 4;

    printf("Number of 32-bit lanes: %lu\n", num_words);

    // Create predicate patterns
    svbool_t pg_all = svptrue_b32();
    svbool_t pg_first4 = svwhilelt_b32(0, 4);

    printf("Created predicate patterns successfully\n");

    // Test simple SVE operation
    printf("\n=== Simple SVE Operation Test ===\n");

    float src[64] __attribute__((aligned(16)));
    float dst[64] __attribute__((aligned(16)));

    // Initialize source
    for (int i = 0; i < 64; ++i) {
        src[i] = static_cast<float>(i);
    }

    // SVE vector load and store
    svfloat32_t vec = svld1_f32(pg_all, src);
    svst1_f32(pg_all, dst, vec);

    // Verify
    bool correct = true;
    for (int i = 0; i < 64; ++i) {
        if (dst[i] != src[i]) {
            correct = false;
            break;
        }
    }

    printf("Load/Store test: %s\n", correct ? "PASSED" : "FAILED");

    // Test predicate masking
    printf("\n=== Predicate Masking Test ===\n");

    for (int i = 0; i < 64; ++i) {
        dst[i] = -1.0f;  // Initialize with -1
    }

    // Load only first 4 elements (predicate controlled)
    svfloat32_t vec_masked = svld1_f32(pg_first4, src);
    svst1_f32(pg_first4, dst, vec_masked);

    // Check: first 4 should be 0,1,2,3; rest should be -1
    correct = true;
    for (int i = 0; i < 4; ++i) {
        if (dst[i] != static_cast<float>(i)) {
            printf("dst[%d] = %f, expected %f\n", i, dst[i], static_cast<float>(i));
            correct = false;
        }
    }
    for (int i = 4; i < 64; ++i) {
        if (dst[i] != -1.0f) {
            printf("dst[%d] = %f, expected -1.0\n", i, dst[i]);
            correct = false;
            break;
        }
    }

    printf("Predicate masking test: %s\n", correct ? "PASSED" : "FAILED");

    printf("\n=== All tests completed ===\n");
    return 0;
}