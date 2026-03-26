cmd_init/main.o := aarch64-linux-gnu-g++ -Wp,-MD,init/.main.o.d -nostdlib -mgeneral-regs-only -ffreestanding -nostartfiles -g -D__KERNEL__ -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -g -Wall -DKBUILD_CONF -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -fno-exceptions -fno-rtti   -o init/main.o -c init/main.cpp

deps_init/main.o := \
  init/main.cpp \
  /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h \
    $(wildcard include/config/h/include//.h) \
    $(wildcard include/config/sys/noirq.h) \
    $(wildcard include/config/sys/poll/rt.h) \
    $(wildcard include/config/sys/task.h) \
  include/benos/benos.hpp \
  include/benos/types.hpp \
  include/benos/error.hpp \
  include/benos/new.hpp \
  include/benos/mm/page_allocator.hpp \
  include/benos/kernel/process.hpp \
  include/benos/kernel/console.hpp \
  /usr/lib/gcc-cross/aarch64-linux-gnu/13/include/stdarg.h \
  include/benos/driver/uart.hpp \
  include/benos/driver/timer.hpp \
  include/asm/sysregs.h \
    $(wildcard include/config/cpu/big/endian.h) \
  include/type.h \
  include/irq.h \
  include/asm/irq.h \
  include/asm/mm.h \
  include/page.h \
  include/sched.h \
  include/asm/system.h \
  arch/arm64/include/asm/mm.h \
  include/asm/processor.h \
  include/timer.h \
    $(wildcard include/config/hz.h) \

init/main.o: $(deps_init/main.o)

$(deps_init/main.o):
