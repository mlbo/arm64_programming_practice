// lab23-4: SVE strcmp Utilities

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace arm64lab::sve {

// SVE strcmp (assembly implementation)
extern "C" auto strcmp_sve(const char* s1, const char* s2) -> int;

} // namespace arm64lab::sve