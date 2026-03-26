cmd_init/main.o := aarch64-linux-gnu-gcc -Wp,-MD,init/.main.o.d -nostdlib -mgeneral-regs-only -ffreestanding -nostartfiles -g -D__KERNEL__ -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/include/uapi -Iinclude/generated/uapi -g -Wall -DKBUILD_CONF -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/include/uapi -Iinclude/generated/uapi -fomit-frame-pointer    -o init/main.o -c init/main.c

deps_init/main.o := \
  init/main.c \
  /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-3/benos/include/target/config.h \
    $(wildcard include/config/h/include//.h) \
    $(wildcard include/config/sys/noirq.h) \
    $(wildcard include/config/sys/poll/rt.h) \
    $(wildcard include/config/sys/task.h) \
  arch/arm64/include/asm/uart.h \
  include/asm/sysregs.h \
    $(wildcard include/config/cpu/big/endian.h) \
  include/type.h \
  include/printk.h \
  include/irq.h \
  include/asm/irq.h \
  include/sched.h \
  include/asm/system.h \
  arch/arm64/include/asm/mm.h \
  include/page.h \
  include/asm/processor.h \
  include/list.h \
  include/timer.h \
    $(wildcard include/config/hz.h) \
  include/asm/current.h \
  include/asm/timer.h \
  include/memory.h \

init/main.o: $(deps_init/main.o)

$(deps_init/main.o):
