cmd_lib/timer_wrapper.o := aarch64-linux-gnu-g++ -Wp,-MD,lib/.timer_wrapper.o.d -nostdlib -mgeneral-regs-only -ffreestanding -nostartfiles -g -D__KERNEL__ -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -g -Wall -DKBUILD_CONF -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -fno-exceptions -fno-rtti   -o lib/timer_wrapper.o -c lib/timer_wrapper.cpp

deps_lib/timer_wrapper.o := \
  lib/timer_wrapper.cpp \
  /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h \
    $(wildcard include/config/h/include//.h) \
    $(wildcard include/config/sys/noirq.h) \
    $(wildcard include/config/sys/poll/rt.h) \
    $(wildcard include/config/sys/task.h) \
  include/benos/driver/timer.hpp \
  include/benos/types.hpp \

lib/timer_wrapper.o: $(deps_lib/timer_wrapper.o)

$(deps_lib/timer_wrapper.o):
