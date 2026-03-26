// asm_test.S - Lab 04-1: Label access test
#define my_label 0x30

.globl string1
string1:
    .string "Boot at EL"

.align 3
.globl my_data
my_data:
    .word 0x44

.align 3
.global access_label_test
access_label_test:
    // 1. Load a large number
    ldr    x1, =0xffff0000ffff0000

    // 2. Initialize with bit operations
    ldr    x2, =(1<<0) | (1<<2) | (1<<20) | (1<<40) | (1<<55)

    // 3. Access a macro definition
    ldr    x0, =my_label      // Load macro value
    ldr    x1, my_label       // PC + macro, load from address

    // 4. Access a string
    ldr    x0, string1        // Load ASCII code
    ldr    x1, =string1       // Load string address

    // 5. Access data
    ldr    x0, my_data        // Load data value
    ldr    x1, =my_data       // Load data address

    ret
