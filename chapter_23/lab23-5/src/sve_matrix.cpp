// lab23-5: SVE Matrix Multiplication Implementation

#include "sve_matrix.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace arm64lab::sve {

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

void matrix_mul_4x4_sve(const float* a, const float* b, float* c) {
    asm volatile (
        "ptrue p0.s, vl4\n"

        // Load matrix A into Z0-Z3
        "ld1w {z0.s}, p0/z, [%[a]]\n"
        "incw %[a], VL4, MUL #4\n"
        "ld1w {z1.s}, p0/z, [%[a]]\n"
        "incw %[a], VL4, MUL #4\n"
        "ld1w {z2.s}, p0/z, [%[a]]\n"
        "incw %[a], VL4, MUL #4\n"
        "ld1w {z3.s}, p0/z, [%[a]]\n"

        // Load matrix B into Z4-Z7
        "ld1w {z4.s}, p0/z, [%[b]]\n"
        "incw %[b], VL4, MUL #4\n"
        "ld1w {z5.s}, p0/z, [%[b]]\n"
        "incw %[b], VL4, MUL #4\n"
        "ld1w {z6.s}, p0/z, [%[b]]\n"
        "incw %[b], VL4, MUL #4\n"
        "ld1w {z7.s}, p0/z, [%[b]]\n"

        // Compute C column 0
        "fmul z8.s, z0.s, z4.s[0]\n"
        "fmla z8.s, z1.s, z4.s[1]\n"
        "fmla z8.s, z2.s, z4.s[2]\n"
        "fmla z8.s, z3.s, z4.s[3]\n"

        // Compute C column 1
        "fmul z9.s, z0.s, z5.s[0]\n"
        "fmla z9.s, z1.s, z5.s[1]\n"
        "fmla z9.s, z2.s, z5.s[2]\n"
        "fmla z9.s, z3.s, z5.s[3]\n"

        // Compute C column 2
        "fmul z10.s, z0.s, z6.s[0]\n"
        "fmla z10.s, z1.s, z6.s[1]\n"
        "fmla z10.s, z2.s, z6.s[2]\n"
        "fmla z10.s, z3.s, z6.s[3]\n"

        // Compute C column 3
        "fmul z11.s, z0.s, z7.s[0]\n"
        "fmla z11.s, z1.s, z7.s[1]\n"
        "fmla z11.s, z2.s, z7.s[2]\n"
        "fmla z11.s, z3.s, z7.s[3]\n"

        // Store result matrix
        "st1w {z8.s}, p0, [%[c]]\n"
        "incw %[c], VL4, MUL #4\n"
        "st1w {z9.s}, p0, [%[c]]\n"
        "incw %[c], VL4, MUL #4\n"
        "st1w {z10.s}, p0, [%[c]]\n"
        "incw %[c], VL4, MUL #4\n"
        "st1w {z11.s}, p0, [%[c]]\n"
        :
        : [a] "r" (a), [b] "r" (b), [c] "r" (c)
        : "memory", "z0", "z1", "z2", "z3",
          "z4", "z5", "z6", "z7", "z8",
          "z9", "z10", "z11", "p0"
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

} // namespace arm64lab::sve