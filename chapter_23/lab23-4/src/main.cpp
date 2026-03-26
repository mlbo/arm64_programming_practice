// lab23-4: SVE strcmp Test
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/auxv.h>

#include <asm/hwcap.h>
#include "sve_strcmp.hpp"

// Import types from namespace

static int strcmp_c(const char* str1, const char* str2) {
    char c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
    } while (c1 != '\0' && c1 == c2);
    return c1 - c2;
}

int main() {
    printf("=== SVE strcmp Test ===\n\n");

    if (!(getauxval(AT_HWCAP) & HWCAP_SVE)) {
        printf("SVE not supported\n");
        return 1;
    }

    struct {
        const char* s1;
        const char* s2;
    } test_cases[] = {
        {"hello", "hello"},
        {"hello", "world"},
        {"hello", "hell"},
        {"hell", "hello"},
        {"", ""},
        {"a", "b"},
        {"b", "a"},
        {"abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz"},
        {"abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxy"},
        {"short", "longer_string"},
        {"same_prefix_different", "same_prefix_another"},
    };

    int passed = 0;
    int total = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < total; ++i) {
        const char* s1 = test_cases[i].s1;
        const char* s2 = test_cases[i].s2;

        int c_result = strcmp_c(s1, s2);
        int sve_result = arm64lab::sve::strcmp_sve(s1, s2);

        // Compare signs (not exact values due to implementation differences)
        bool match = (c_result == 0 && sve_result == 0) ||
                     (c_result < 0 && sve_result < 0) ||
                     (c_result > 0 && sve_result > 0);

        printf("Test %d: \"%s\" vs \"%s\"\n", i + 1, s1, s2);
        printf("  C:    %d\n", c_result);
        printf("  SVE:  %d\n", sve_result);
        printf("  %s\n\n", match ? "MATCH" : "MISMATCH");

        if (match) ++passed;
    }

    printf("=== Results ===\n");
    printf("Passed: %d/%d\n", passed, total);

    return (passed == total) ? 0 : 1;
}