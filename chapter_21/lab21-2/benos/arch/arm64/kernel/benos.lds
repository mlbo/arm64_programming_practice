/*
 * Automatically generated C config: don't edit
 * BenOS version: 1.0.0
 * Thu Mar 26 21:35:05 2026
 */
/* Three system performance ? should be considered carefully to choose the
 * software architecture model:
 * 1. throughput: IO throughput should be as great as possible
 * 2. realtime: time critical tasks can be run on time
 * 3. faireness: tasks can be distributed fairly on difference computing
 *               resources
 * Systems will always take throughput into account, realtime and fairness
 * are left for users' choices.
 */
/* to align the pointer to the (next) page boundary */
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
  . = ALIGN((1 << 12));
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
