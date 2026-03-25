#include <page.h>

OUTPUT_ARCH(aarch64)
ENTRY(_start)

SECTIONS
{
	. = 0x80000;
	_text_boot = .;
	.text.boot : {
		*(.text.boot)
	}
	_etext_boot = .;
	_text = .;
	.text : {
		*(.text)
	}
	_etext = .;

	_rodata = .;
	.rodata : {
		*(.rodata)
	}
	_erodata = .;
	_data = .;
	.data : {
		*(.data)
		. = ALIGN(PAGE_SIZE);
		*(.data.init_task)
	}

	/* C++ init array for global constructors */
	. = ALIGN(8);
	__init_array_start = .;
	.init_array : {
		*(.init_array)
		*(.init_array.*)
	}
	__init_array_end = .;

	. = ALIGN(0x8);
	_edata = .;
	_bss = .;
	bss_begin = .;
	.bss : { *(.bss*) }
	bss_end = .;
	_ebss = .;

}
