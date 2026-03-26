/*
 * Automatically generated C config: don't edit
 * BenOS version: 1.0.0
 * Sun May 15 12:56:54 2022
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
 . = ALIGN(0x8);
 _edata = .;
 _bss = .;
 bss_begin = .;
 .bss : { *(.bss*) }
 bss_end = .;
 _ebss = .;
}
