// lab23-1: SVE Basic Implementation

#include "sve_basic.hpp"
#include <stdio.h>
#include <sys/auxv.h>
#include <asm/hwcap.h>

namespace arm64lab::sve {

auto has_sve_privileged() -> bool {
    u64 id_aa64pfr0;
    asm volatile("mrs %0, ID_AA64PFR0_EL1" : "=r"(id_aa64pfr0));
    // Bits [35:32] contain the SVE field
    return ((id_aa64pfr0 >> 32) & 0xF) != 0;
}

auto has_sve() -> bool {
    // Use getauxval for user-mode detection
    return (getauxval(AT_HWCAP) & HWCAP_SVE) != 0;
}

auto get_vector_length() -> u64 {
    u64 vl;
    asm volatile("rdvl %0, #1" : "=r"(vl));
    return vl;  // Returns length in bytes
}

auto get_vector_length_bits() -> u64 {
    return get_vector_length() * 8;
}

void print_vector_info() {
    if (has_sve()) {
        u64 vl_bytes = get_vector_length();
        u64 vl_bits = vl_bytes * 8;
        printf("SVE is supported\n");
        printf("Vector length: %lu bytes (%lu bits)\n", vl_bytes, vl_bits);
    } else {
        printf("SVE is NOT supported on this system\n");
    }
}

} // namespace arm64lab::sve