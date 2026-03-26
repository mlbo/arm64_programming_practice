# 代码说明

## EL 切换实现

### 从 EL1 切换到 EL0

```cpp
// exception.hpp
namespace benos::arch::arm64 {

// SPSR 模式定义
enum : u64 {
    SPSR_EL0t = 0x0000,  // EL0, 使用 SP_EL0
    SPSR_EL1t = 0x0004,  // EL1, 使用 SP_EL0
    SPSR_EL1h = 0x0005,  // EL1, 使用 SP_EL1
    SPSR_EL2t = 0x0008,  // EL2, 使用 SP_EL0
    SPSR_EL2h = 0x0009,  // EL2, 使用 SP_EL2
};

// 切换到 EL0
void switch_to_el0(void (*entry)(), void* stack_top);

}  // namespace benos::arch::arm64
```

```cpp
// exception.cpp
namespace benos::arch::arm64 {

void switch_to_el0(void (*entry)(), void* stack_top) {
    // 禁用中断
    asm volatile("msr daifset, #2");

    // 设置用户栈
    asm volatile("msr sp_el0, %0" :: "r" (stack_top));

    // 设置返回地址
    asm volatile("msr elr_el1, %0" :: "r" (reinterpret_cast<u64>(entry)));

    // 设置 SPSR (EL0t 模式，启用所有中断)
    u64 spsr = SPSR_EL0t;
    asm volatile("msr spsr_el1, %0" :: "r" (spsr));

    // 执行 eret，跳转到 EL0
    asm volatile("eret");
}

}  // namespace benos::arch::arm64
```

## 用户态程序

```cpp
// user_main.cpp
void user_main() {
    // 现在运行在 EL0
    // 只能通过 syscall 与内核交互

    while (true) {
        // 用户循环
        asm volatile("svc #0");  // 系统调用
    }
}
```

## 内核启动流程

```cpp
// kernel.cpp
extern "C" void kernel_main() {
    // 初始化 UART 和控制台
    uart_init();
    console_init();

    // 设置异常向量表
    init_exception_vector();

    printk("Starting from EL1\n");

    // 定义用户栈
    static u8 user_stack[4096];

    // 切换到 EL0
    switch_to_el0(user_main, user_stack + sizeof(user_stack));

    // 不应该到达这里
    while (true) {
        asm volatile("wfi");
    }
}
```

## 系统调用处理

```asm
// entry.S
el0_sync:
    kernel_entry 0
    mrs x0, esr_el1
    lsr x0, x0, #26
    cmp x0, #0x15       ; SVC 指令
    b.ne el0_sync_invalid
    bl handle_syscall
    kernel_exit 0
```