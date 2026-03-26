cmd_kernel/process_manager.o := aarch64-linux-gnu-g++ -Wp,-MD,kernel/.process_manager.o.d -nostdlib -mgeneral-regs-only -ffreestanding -nostartfiles -g -D__KERNEL__ -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -g -Wall -DKBUILD_CONF -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -fno-exceptions -fno-rtti   -o kernel/process_manager.o -c kernel/process_manager.cpp

deps_kernel/process_manager.o := \
  kernel/process_manager.cpp \
  /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h \
    $(wildcard include/config/h/include//.h) \
    $(wildcard include/config/sys/noirq.h) \
    $(wildcard include/config/sys/poll/rt.h) \
    $(wildcard include/config/sys/task.h) \
  include/benos/kernel/process.hpp \
  include/benos/types.hpp \
  include/sched.h \
  include/asm/system.h \
  include/type.h \
  arch/arm64/include/asm/mm.h \
  include/page.h \
  include/asm/processor.h \
  include/timer.h \
    $(wildcard include/config/hz.h) \

kernel/process_manager.o: $(deps_kernel/process_manager.o)

$(deps_kernel/process_manager.o):
