# LDXR/STXR 原子操作知识点

## 1. 独占访问指令

### LDXR (Load Exclusive Register)

```asm
ldxr x0, [x1]     ; 独占加载，标记地址为"独占访问"
```

### STXR (Store Exclusive Register)

```asm
stxr w2, x0, [x1] ; 独占存储，成功则 w2=0，失败则 w2=1
```

## 2. 原子加法实现

```cpp
void atomic_add(u32* addr, u32 delta) {
    u32 tmp, result;

    asm volatile (
        "1: ldxr %w0, [%2]\n"      // 独占加载
        "   add  %w0, %w0, %w3\n"  // 加法
        "   stxr %w1, %w0, [%2]\n" // 独占存储
        "   cbnz %w1, 1b\n"        // 失败重试
        : "=&r" (result), "=&r" (tmp)
        : "r" (addr), "r" (delta)
        : "memory"
    );
}
```

## 3. 自旋锁

```cpp
class SpinLock {
public:
    void lock() {
        u32 tmp;
        asm volatile (
            "1: ldaxr %w0, [%1]\n"    // 独占加载，带获取语义
            "   cbnz %w0, 1b\n"       // 已锁定，等待
            "   stxr %w0, %w2, [%1]\n" // 尝试锁定
            "   cbnz %w0, 1b\n"       // 失败重试
            : "=&r" (tmp)
            : "r" (&locked_), "r" (1)
            : "memory"
        );
    }

    void unlock() {
        asm volatile (
            "stlr %w0, [%1]"  // 带释放语义的存储
            :
            : "r" (0), "r" (&locked_)
            : "memory"
        );
    }

private:
    volatile u32 locked_ = 0;
};
```

## 4. 内存屏障变体

- `ldxr/stxr`: 无内存屏障
- `ldaxr/stlxr`: 带 Acquire/Release 语义
- `ldaxr/stlr`: 混合使用