# 知识点：原子操作正确实现

## 1. 正确的原子写入

```asm
// 原子写入：将 new_val 写入 addr
atomic_store:
    // x0 = addr, x1 = new_val
1:
    ldxr x2, [x0]      // 独占加载（值被忽略）
    stxr w3, x1, [x0]  // 存储 new_val
    cbnz w3, 1b
    ret
```

## 2. 原子交换

```asm
// 原子交换：返回旧值，写入新值
atomic_swap:
    // x0 = addr, x1 = new_val
1:
    ldxr x2, [x0]      // 加载旧值
    stxr w3, x1, [x0]  // 存储新值
    cbnz w3, 1b
    mov x0, x2         // 返回旧值
    ret
```

## 3. 原子加法

```asm
// 原子加法：返回旧值
atomic_fetch_add:
    // x0 = addr, x1 = delta
1:
    ldxr x2, [x0]      // 加载旧值
    add x2, x2, x1     // 计算新值
    stxr w3, x2, [x0]  // 存储新值
    cbnz w3, 1b
    sub x0, x2, x1     // 返回旧值
    ret
```

## 4. 比较并交换 (CAS)

```asm
// 原子 CAS：如果 *addr == expected，则写入 new_val
// 返回旧值
atomic_cas:
    // x0 = addr, x1 = expected, x2 = new_val
1:
    ldxr x3, [x0]
    cmp x3, x1
    b.ne 2f
    stxr w4, x2, [x0]
    cbnz w4, 1b
2:
    mov x0, x3
    ret
```

## 5. LSE 原子指令

ARMv8.1-A 引入了 LSE 扩展，提供更高效的原子指令：

```asm
// 使用 LSE 指令
stadd x1, [x0]     // 原子加法
swp x1, x2, [x0]   // 原子交换
cas x1, x2, [x0]   // 比较并交换
```