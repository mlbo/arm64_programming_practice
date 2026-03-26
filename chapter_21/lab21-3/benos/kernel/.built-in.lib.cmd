cmd_kernel/built-in.lib :=  aarch64-linux-gnu-ld   -r -o kernel/built-in.lib kernel/fork.o kernel/sched.o kernel/sched_simple.o kernel/irq.o
