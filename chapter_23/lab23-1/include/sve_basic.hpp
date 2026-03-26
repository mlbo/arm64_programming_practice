// lab23-1: SVE Basic Utilities

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace arm64lab::sve {

using u8 = uint8_t;
using u64 = uint64_t;
using usize = size_t;

// Check if SVE is supported (requires privilege for MRS)
auto has_sve_privileged() -> bool;

// Check if SVE is supported (user mode via getauxval)
auto has_sve() -> bool;

// Get current vector length in bytes
auto get_vector_length() -> u64;

// Get vector length in bits
auto get_vector_length_bits() -> u64;

// Print vector length info
void print_vector_info();

} // namespace arm64lab::sve