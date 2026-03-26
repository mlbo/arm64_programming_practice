/* Linker script for Raspberry Pi */

ENTRY(_start)

SECTIONS
{
    /* Raspberry Pi loads kernel at 0x80000 */
    . = 0x80000;

    /* Boot code must be first */
    .text.boot : { *(.text.boot) }

    /* Regular code */
    .text : { *(.text*) }

    /* Read-only data */
    .rodata : { *(.rodata*) }

    /* Initialized data */
    .data : { *(.data*) }

    /* C++ constructors */
    .init_array : {
        _init_array_start = .;
        KEEP(*(.init_array*))
        _init_array_end = .;
    }

    /* BSS section */
    .bss : {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    }

    /* Stack */
    . = ALIGN(16);
    . = . + 0x10000;  /* 64KB stack */
    _stack_top = .;

    /* Discard unnecessary sections */
    /DISCARD/ : {
        *(.comment)
        *(.note*)
        *(.eh_frame*)
    }
}