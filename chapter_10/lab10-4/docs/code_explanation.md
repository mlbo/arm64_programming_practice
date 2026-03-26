# 代码说明

## LSE 原子操作实现

```cpp
namespace benos {

// 检测 LSE 支持
inline auto has_lse() -> bool {
    u64 isar0;
    asm volatile("mrs %0, ID_AA64ISAR0_EL1" : "=r" (isar0));
    return ((isar0 >> 20) & 0xF) != 0;
}

// 使用 LSE 的原子加法
void atomic_add_lse(u32* addr, u32 delta) {
    asm volatile (
        "staddal %w0, [%1]"
        :
        : "r" (delta), "r" (addr)
        : "memory"
    );
}

// 使用 LSE 的原子或
void atomic_or_lse(u32* addr, u32 mask) {
    asm volatile (
        "ldsetal %w0, wzr, [%1]"
        :
        : "r" (mask), "r" (addr)
        : "memory"
    );
}

// 使用 LSE 的比较交换
auto cas_lse(u32* addr, u32 expected, u32 new_val) -> bool {
    u32 result;
    asm volatile (
        "casal %w1, %w2, [%3]"
        : "=r" (result)
        : "r" (expected), "r" (new_val), "r" (addr)
        : "memory"
    );
    return result == expected;
}

// 自适应原子操作
class Atomic {
public:
    static void add(u32* addr, u32 delta) {
        if (has_lse()) {
            atomic_add_lse(addr, delta);
        } else {
            atomic_add_llsc(addr, delta);
        }
    }

private:
    static void atomic_add_llsc(u32* addr, u32 delta) {
        u32 tmp, result;
        asm volatile (
            "1: ldxr %w0, [%2]\n"
            "   add  %w0, %w0, %w3\n"
            "   stxr %w1, %w0, [%2]\n"
            "   cbnz %w1, 1b"
            : "=&r" (result), "=&r" (tmp)
            : "r" (addr), "r" (delta)
            : "memory"
        );
    }
};

}  // namespace benos
```