// lab22-2: NEON Matrix Multiplication Utilities

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace arm64lab::neon {

using u32 = uint32_t;

// 4x4 Matrix multiplication (scalar version)
void matrix_mul_4x4_scalar(const float* a, const float* b, float* c);

// 4x4 Matrix multiplication (NEON intrinsics version)
void matrix_mul_4x4_neon(const float* a, const float* b, float* c);

// 4x4 Matrix multiplication (inline assembly version)
void matrix_mul_4x4_asm(const float* a, const float* b, float* c);

// Utility functions
void matrix_print(const float* m, u32 cols, u32 rows);
void matrix_init_random(float* m, u32 count);
void matrix_init_value(float* m, u32 cols, u32 rows, float val);
bool matrix_compare(const float* a, const float* b, u32 rows, u32 cols);

} // namespace arm64lab::neon