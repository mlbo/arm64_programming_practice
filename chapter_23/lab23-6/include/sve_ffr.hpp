// lab23-6: SVE FFR (First Fault Register) Utilities

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab::sve {

using u8 = uint8_t;
using usize = size_t;

// Demonstrate FFR initialization
void ffr_init_demo();

// Process unknown-length array using FFR
void process_unknown_length(const u8* data, u8* result, usize max_bytes);

} // namespace arm64lab::sve