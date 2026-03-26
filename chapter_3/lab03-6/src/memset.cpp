// memset.cpp - memset implementation
#include "memset.hpp"

namespace arm64lab {

static void* __memset_1bytes(void* s, int c, usize count) {
    char* xs = static_cast<char*>(s);
    while (count--) {
        *xs++ = static_cast<char>(c);
    }
    return s;
}

void* memset(void* s, int c, usize count) {
    char* p = static_cast<char*>(s);
    constexpr usize align = 16;
    usize size, left = count;

    // Build 64-bit value from byte c
    u64 data = 0;
    for (int i = 0; i < 8; i++) {
        data |= static_cast<u64>(c & 0xFF) << (i * 8);
    }

    // Check 16-byte alignment
    auto address = reinterpret_cast<u64>(p);
    if (address & (align - 1)) {
        size = align - (address & (align - 1));
        __memset_1bytes(p, c, size);
        p += size;
        left = count - size;
    }

    // Fill 16 bytes at a time
    if (left > align) {
        usize n = left / align;
        left = left % align;

        __memset_16bytes(p, data, 16 * n);

        if (left) {
            __memset_1bytes(p + 16 * n, c, left);
        }
    }

    return s;
}

} // namespace arm64lab