// lab22-2: NEON Matrix Multiplication Implementation

#include "neon_matrix.hpp"
#include <arm_neon.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace arm64lab::neon {

void matrix_mul_4x4_scalar(const float* a, const float* b, float* c) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            c[4 * j + i] = 0;
            for (int k = 0; k < 4; ++k) {
                c[4 * j + i] += a[4 * k + i] * b[4 * j + k];
            }
        }
    }
}

void matrix_mul_4x4_neon(const float* a, const float* b, float* c) {
    // Load columns of matrix B
    float32x4_t b0 = vld1q_f32(b);      // Column 0
    float32x4_t b1 = vld1q_f32(b + 4);  // Column 1
    float32x4_t b2 = vld1q_f32(b + 8);  // Column 2
    float32x4_t b3 = vld1q_f32(b + 12); // Column 3

    // Compute each row of result
    for (int i = 0; i < 4; ++i) {
        float32x4_t a_row = vld1q_f32(a + i * 4);

        // Use FMA to compute result
        float32x4_t r = vmulq_laneq_f32(b0, a_row, 0);
        r = vfmaq_laneq_f32(r, b1, a_row, 1);
        r = vfmaq_laneq_f32(r, b2, a_row, 2);
        r = vfmaq_laneq_f32(r, b3, a_row, 3);

        vst1q_f32(c + i * 4, r);
    }
}

void matrix_mul_4x4_asm(const float* a, const float* b, float* c) {
    asm volatile (
        // Load matrix A into v0-v3
        "ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [%[a]]\n"
        // Load matrix B into v4-v7
        "ld1 {v4.4s, v5.4s, v6.4s, v7.4s}, [%[b]]\n"

        // Initialize result registers
        "movi v8.4s,  #0\n"
        "movi v9.4s,  #0\n"
        "movi v10.4s, #0\n"
        "movi v11.4s, #0\n"

        // Compute C column 0
        "fmla v8.4s, v0.4s, v4.s[0]\n"
        "fmla v8.4s, v1.4s, v4.s[1]\n"
        "fmla v8.4s, v2.4s, v4.s[2]\n"
        "fmla v8.4s, v3.4s, v4.s[3]\n"

        // Compute C column 1
        "fmla v9.4s, v0.4s, v5.s[0]\n"
        "fmla v9.4s, v1.4s, v5.s[1]\n"
        "fmla v9.4s, v2.4s, v5.s[2]\n"
        "fmla v9.4s, v3.4s, v5.s[3]\n"

        // Compute C column 2
        "fmla v10.4s, v0.4s, v6.s[0]\n"
        "fmla v10.4s, v1.4s, v6.s[1]\n"
        "fmla v10.4s, v2.4s, v6.s[2]\n"
        "fmla v10.4s, v3.4s, v6.s[3]\n"

        // Compute C column 3
        "fmla v11.4s, v0.4s, v7.s[0]\n"
        "fmla v11.4s, v1.4s, v7.s[1]\n"
        "fmla v11.4s, v2.4s, v7.s[2]\n"
        "fmla v11.4s, v3.4s, v7.s[3]\n"

        // Store result matrix
        "st1 {v8.4s, v9.4s, v10.4s, v11.4s}, [%[c]]\n"
        :
        : [a] "r" (a), [b] "r" (b), [c] "r" (c)
        : "memory", "v0", "v1", "v2", "v3",
          "v4", "v5", "v6", "v7", "v8",
          "v9", "v10", "v11"
    );
}

void matrix_print(const float* m, u32 cols, u32 rows) {
    for (u32 i = 0; i < rows; ++i) {
        for (u32 j = 0; j < cols; ++j) {
            printf("%f ", m[j * rows + i]);
        }
        printf("\n");
    }
    printf("\n");
}

void matrix_init_random(float* m, u32 count) {
    for (u32 i = 0; i < count; ++i) {
        m[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
}

void matrix_init_value(float* m, u32 cols, u32 rows, float val) {
    for (u32 i = 0; i < rows; ++i) {
        for (u32 j = 0; j < cols; ++j) {
            m[j * rows + i] = val;
        }
    }
}

bool matrix_compare(const float* a, const float* b, u32 rows, u32 cols) {
    for (u32 i = 0; i < rows; ++i) {
        for (u32 j = 0; j < cols; ++j) {
            float va = a[rows * j + i];
            float vb = b[rows * j + i];
            if (fabs(va - vb) > 0.000001f) {
                printf("Mismatch at [%u][%u]: A=%f, B=%f\n", i, j, va, vb);
                return false;
            }
        }
    }
    return true;
}

} // namespace arm64lab::neon