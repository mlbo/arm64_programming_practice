# 代码说明

## 读取 CurrentEL

### 汇编实现

```asm
get_current_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    and x0, x0, #3
    ret
```

### C++ 实现

```cpp
namespace benos::arch::arm64 {

enum class ExceptionLevel : u32 {
    EL0 = 0,
    EL1 = 1,
    EL2 = 2,
    EL3 = 3,
};

auto current_el() -> ExceptionLevel {
    u64 el;
    asm volatile("mrs %0, CurrentEL" : "=r" (el));
    return static_cast<ExceptionLevel>((el >> 2) & 0x3);
}

const char* el_to_string(ExceptionLevel el) {
    switch (el) {
    case ExceptionLevel::EL0: return "EL0 (User)";
    case ExceptionLevel::EL1: return "EL1 (Kernel)";
    case ExceptionLevel::EL2: return "EL2 (Hypervisor)";
    case ExceptionLevel::EL3: return "EL3 (Secure Monitor)";
    default: return "Unknown";
    }
}

}  // namespace benos::arch::arm64
```

## 测试代码

```cpp
void kernel_main() {
    uart_init();
    console_init();

    auto el = benos::arch::arm64::current_el();
    printk("Current Exception Level: %s\n",
           benos::arch::arm64::el_to_string(el));

    // 在 QEMU 中通常从 EL3 或 EL2 启动
    // 可以逐步降低到 EL1
}
```