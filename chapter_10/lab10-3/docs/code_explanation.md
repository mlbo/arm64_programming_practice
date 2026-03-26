# 代码说明

## Atomic 模板类

```cpp
namespace benos {

template<typename T>
class Atomic {
public:
    explicit Atomic(T val = 0) : value_(val) {}

    auto load() const -> T {
        T result;
        asm volatile (
            "ldxr %w0, [%1]"
            : "=r" (result)
            : "r" (&value_)
            : "memory"
        );
        return result;
    }

    void store(T val) {
        u32 tmp;
        asm volatile (
            "1: stxr %w0, %w2, [%1]\n"
            "   cbnz %w0, 1b"
            : "=&r" (tmp)
            : "r" (&value_), "r" (val)
            : "memory"
        );
    }

    auto fetch_add(T delta) -> T {
        T old_val, new_val;
        u32 tmp;
        asm volatile (
            "1: ldxr %w0, [%3]\n"
            "   add  %w1, %w0, %w4\n"
            "   stxr %w2, %w1, [%3]\n"
            "   cbnz %w2, 1b"
            : "=&r" (old_val), "=&r" (new_val), "=&r" (tmp)
            : "r" (&value_), "r" (delta)
            : "memory"
        );
        return old_val;
    }

private:
    volatile T value_;
};

}  // namespace benos
```