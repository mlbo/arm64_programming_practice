// lab23-6: SVE FFR Test
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/auxv.h>
#include <arm_sve.h>
#include <asm/hwcap.h>
#include "sve_ffr.hpp"

// A simple strlen using FFR concept (simplified version)
size_t strlen_sve_simple(const char* s) {
    size_t len = 0;

    while (true) {
        // Use SVE to check for '\0' in chunks
        svbool_t pg = svptrue_b8();
        svuint8_t vec = svld1_u8(pg, reinterpret_cast<const arm64lab::sve::u8*>(s + len));

        // Find '\0'
        svbool_t found = svcmpeq_n_u8(pg, vec, 0);

        // Check if we found '\0'
        if (svptest_any(pg, found)) {
            // Find position of first '\0'
            // Use cntp to count active lanes before '\0'
            svbool_t before_null = svbrkb_z(pg, found);
            len += svcntp_b8(pg, before_null);
            return len;
        }

        len += svcntb();
    }
}

int main() {
    printf("=== SVE FFR (First Fault Register) Test ===\n\n");

    if (!(getauxval(AT_HWCAP) & HWCAP_SVE)) {
        printf("SVE not supported\n");
        return 1;
    }

    // Test 1: FFR initialization
    printf("=== Test 1: FFR Initialization ===\n");
    arm64lab::sve::ffr_init_demo();
    printf("FFR initialized successfully\n\n");

    // Test 2: Process unknown length array
    printf("=== Test 2: Process Unknown Length Array ===\n");
    constexpr size_t DATA_SIZE = 256;

    arm64lab::sve::u8* data = static_cast<arm64lab::sve::u8*>(malloc(DATA_SIZE));
    arm64lab::sve::u8* result = static_cast<arm64lab::sve::u8*>(malloc(DATA_SIZE));

    if (!data || !result) {
        printf("Failed to allocate buffers\n");
        return 1;
    }

    // Initialize data
    for (size_t i = 0; i < DATA_SIZE; ++i) {
        data[i] = static_cast<arm64lab::sve::u8>(i);
    }

    // Process
    arm64lab::sve::process_unknown_length(data, result, DATA_SIZE);

    // Verify
    bool success = true;
    for (size_t i = 0; i < DATA_SIZE; ++i) {
        arm64lab::sve::u8 expected = static_cast<arm64lab::sve::u8>(i + 1);
        if (result[i] != expected) {
            printf("Mismatch at %zu: expected %u, got %u\n",
                   i, expected, result[i]);
            success = false;
        }
    }
    printf("Process unknown length: %s\n\n", success ? "PASSED" : "FAILED");

    // Test 3: String length with SVE
    printf("=== Test 3: String Length with SVE ===\n");
    const char* test_strings[] = {
        "hello",
        "world!",
        "This is a longer string for testing",
        "a",
        ""
    };

    success = true;
    for (const char* str : test_strings) {
        size_t sve_len = strlen_sve_simple(str);
        size_t c_len = strlen(str);

        printf("String: \"%s\"\n", str);
        printf("  SVE strlen: %zu\n", sve_len);
        printf("  C strlen:   %zu\n", c_len);

        if (sve_len != c_len) {
            printf("  MISMATCH!\n");
            success = false;
        }
        printf("\n");
    }

    printf("String length test: %s\n\n", success ? "PASSED" : "FAILED");

    // Cleanup
    free(data);
    free(result);

    printf("=== All FFR tests completed ===\n");
    return 0;
}