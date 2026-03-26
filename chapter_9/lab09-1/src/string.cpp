// String utilities implementation

#include "string.hpp"

namespace benos {

auto strlen(const char* str) -> usize {
    const char* s = str;
    while (*s) ++s;
    return static_cast<usize>(s - str);
}

auto strcpy(char* dst, const char* src) -> char* {
    char* d = dst;
    while ((*d++ = *src++) != '\0');
    return dst;
}

auto strcmp(const char* s1, const char* s2) -> int {
    while (*s1 && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return static_cast<int>(*reinterpret_cast<const u8*>(s1)) -
           static_cast<int>(*reinterpret_cast<const u8*>(s2));
}

void memset(void* dst, int val, usize count) {
    auto d = static_cast<u8*>(dst);
    while (count--) {
        *d++ = static_cast<u8>(val);
    }
}

void memcpy(void* dst, const void* src, usize count) {
    auto d = static_cast<u8*>(dst);
    auto s = static_cast<const u8*>(src);
    while (count--) {
        *d++ = *s++;
    }
}

auto memcmp(const void* s1, const void* s2, usize count) -> int {
    auto p1 = static_cast<const u8*>(s1);
    auto p2 = static_cast<const u8*>(s2);

    while (count--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        ++p1;
        ++p2;
    }
    return 0;
}

}  // namespace benos