// String utilities for bare metal

#pragma once

#include "../../shared/baremetal/types.hpp"

namespace benos {

// String functions
auto strlen(const char* str) -> usize;
auto strcpy(char* dst, const char* src) -> char*;
auto strcmp(const char* s1, const char* s2) -> int;

// Memory functions
void memset(void* dst, int val, usize count);
void memcpy(void* dst, const void* src, usize count);
auto memcmp(const void* s1, const void* s2, usize count) -> int;

}  // namespace benos