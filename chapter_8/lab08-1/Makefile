# 用户态汇编程序
ARMGNU ?= aarch64-linux-gnu

COPS = -g -Wall
ASMOPS = -g -D__ASSEMBLY__

all: test

test: test.S
	$(ARMGNU)-gcc $(ASMOPS) -o test test.S

clean:
	rm -f test *.o

run: test
	./test

.PHONY: all clean run