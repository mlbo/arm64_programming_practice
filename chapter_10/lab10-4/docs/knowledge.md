# LSE 原子指令知识点

## 1. LSE 指令集

ARMv8.1-A 引入了 Large System Extensions (LSE)，提供硬件原子操作指令。

### 原子内存操作

| 指令 | 功能 |
|------|------|
| `LDADD` | 原子加并返回旧值 |
| `LDSET` | 原子或并返回旧值 |
| `LDCLR` | 原子与（取反）并返回旧值 |
| `LDEOR` | 原子异或并返回旧值 |
| `SWP` | 原子交换 |

### 带后缀变体

- `L` 后缀: 带 Acquire 语义 (如 `LDADDAL`)
- `S` 后缀: 带 Release 语义 (如 `LDADDL`)
- `A` 后缀: 带内存屏障 (如 `LDADDA`)

## 2. 常用指令示例

### 原子加法

```asm
ldaddal w0, w1, [x2]  ; 原子加法，返回旧值到 w1
```

```cpp
auto atomic_fetch_add(u32* addr, u32 delta) -> u32 {
    u32 result;
    asm volatile (
        "ldaddal %w1, %w0, [%2]"
        : "=r" (result)
        : "r" (delta), "r" (addr)
        : "memory"
    );
    return result;
}
```

### 原子或

```asm
ldsetal w0, w1, [x2]  ; 原子或，返回旧值
```

### 原子交换

```asm
swpal w0, w1, [x2]    ; 原子交换，返回旧值
```

## 3. CAS (Compare and Swap)

```asm
casal w0, w1, [x2]    ; 如果 [x2] == w0，则 [x2] = w1
```

```cpp
auto compare_and_swap(u32* addr, u32 expected, u32 new_val) -> bool {
    u32 result;
    asm volatile (
        "casal %w1, %w2, [%3]"
        : "=r" (result)
        : "r" (expected), "r" (new_val), "r" (addr)
        : "memory"
    );
    return result == expected;
}
```

## 4. 兼容性处理

运行时检测 LSE 支持：

```cpp
bool has_lse() {
    u64 isar0;
    asm volatile("mrs %0, ID_AA64ISAR0_EL1" : "=r" (isar0));
    return (isar0 >> 20) & 0xF;  // Atomic bits
}
```

如果没有 LSE 支持，回退到 LL/SC：

```cpp
void atomic_add(u32* addr, u32 delta) {
    if (has_lse()) {
        asm volatile("staddal %w0, [%1]" : : "r" (delta), "r" (addr) : "memory");
    } else {
        // LL/SC fallback
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
}
```