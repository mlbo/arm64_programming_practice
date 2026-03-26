// lab23-3: SVE memcpy Implementation (Intrinsics)

#include "sve_memcpy.hpp"
#include <arm_sve.h>

namespace arm64lab::sve {

void sve_memcpy_intr(void* dst, const void* src, usize n) {
    u8* d = static_cast<u8*>(dst);
    const u8* s = static_cast<const u8*>(src);
    u64 i = 0;

    while (i < n) {
        svbool_t pg = svwhilelt_b8(i, n);
        svuint8_t data = svld1_u8(pg, s + i);
        svst1_u8(pg, d + i, data);
        i += svcntb();
    }
}

} // namespace arm64lab::sve