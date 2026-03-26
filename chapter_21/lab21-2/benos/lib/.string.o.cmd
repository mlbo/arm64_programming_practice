cmd_lib/string.o := aarch64-linux-gnu-gcc -Wp,-MD,lib/.string.o.d -nostdlib -mgeneral-regs-only -ffreestanding -nostartfiles -g -D__KERNEL__ -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -g -Wall -DKBUILD_CONF -Iinclude -Iinclude/std -include /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h  -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include -Iarch/arm64/include/generated/uapi -Iarch/arm64/include/generated -Iarch/arm64/include/asm  -Iinclude -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/arch/arm64/include/uapi -Iarch/arm64/include/generated/uapi -I/home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/uapi -Iinclude/generated/uapi -fomit-frame-pointer    -o lib/string.o -c lib/string.c

deps_lib/string.o := \
  lib/string.c \
  /home/mlbo/claude/arm64_programming_practice/chapter_21/lab21-2/benos/include/target/config.h \
    $(wildcard include/config/h/include//.h) \
    $(wildcard include/config/sys/noirq.h) \
    $(wildcard include/config/sys/poll/rt.h) \
    $(wildcard include/config/sys/task.h) \
  include/type.h \

lib/string.o: $(deps_lib/string.o)

$(deps_lib/string.o):
