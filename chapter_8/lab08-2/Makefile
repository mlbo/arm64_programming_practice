# 用户态混合编程
ARMGNU ?= aarch64-linux-gnu

COPS = -g -Wall -static
ASMOPS = -g -D__ASSEMBLY__

all: main

main: main.c compare.S
	$(ARMGNU)-gcc $(COPS) -o main main.c compare.S

clean:
	rm -f main *.o

run: main
	./main

.PHONY: all clean run