/* Linker script for Raspberry Pi */

ENTRY(_start)

SECTIONS
{
    /* Raspberry Pi loads kernel at 0x80000 */
    . = 0x80000;

    /* Boot code must be first */
    .text.boot : {
        _text_boot = .;
        *(.text.boot)
        _etext_boot = .;
    }

    /* Regular code */
    .text : {
        _text = .;
        *(.text*)
        _etext = .;
    }

    /* Read-only data */
    .rodata : {
        _rodata = .;
        *(.rodata*)
        _erodata = .;
    }

    /* Initialized data */
    .data : {
        _data = .;
        *(.data*)
        _edata = .;
    }

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