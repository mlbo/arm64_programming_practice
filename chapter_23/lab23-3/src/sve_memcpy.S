// lab23-3: SVE memcpy Assembly Implementation

.global sve_memcpy_1b
sve_memcpy_1b:
    mov     x3, #0
    whilelt p0.b, x3, x2

1:
    ld1b    z0.b, p0/z, [x1, x3]
    st1b    z0.b, p0, [x0, x3]
    incb    x3
    whilelt p0.b, x3, x2
    b.any   1b

    ret

.global sve_memcpy_4b
sve_memcpy_4b:
    lsr     x2, x2, #2
    mov     x3, #0
    whilelt p0.s, x3, x2

1:
    ld1w    z0.s, p0/z, [x1, x3, lsl 2]
    st1w    z0.s, p0, [x0, x3, lsl 2]
    incw    x3
    whilelt p0.s, x3, x2
    b.any   1b

    ret