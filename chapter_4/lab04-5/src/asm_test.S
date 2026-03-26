// asm_test.S - Lab 04-5: BFI/UBFX/SBFX bitfield instructions

.global bitfield_test
bitfield_test:
    // BFI: Bit Field Insert
    mov    x1, 0x345
    mov    x0, 0
    bfi    x0, x1, 8, 4        // Insert 4 bits from x1[3:0] to x0[11:8]

    // UBFX: Unsigned Bit Field Extract
    ldr    x2, =0x5678abcd
    ubfx   x3, x2, #4, #8      // Extract 8 bits from x2[11:4], zero-extend

    // SBFX: Signed Bit Field Extract
    sbfx   x4, x2, #4, #8      // Extract 8 bits from x2[11:4], sign-extend

    // Read CPU feature register using UBFX
    mrs    x1, ID_AA64ISAR0_EL1

    // Extract atomic field (bits [23:20])
    ubfx   x0, x1, #20, #4

    // Extract AES field (bits [7:4])
    ubfx   x2, x1, #4, #4

    ret
