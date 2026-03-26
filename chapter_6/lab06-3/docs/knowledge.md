# 知识点：原子操作

## 1. LDXR/STXR 指令

### LDXR (Load Exclusive)

```asm
ldxr x0, [x1]    // 独占加载，标记地址为"被监视"
```

### STXR (Store Exclusive)

```asm
stxr w0, x2, [x1]    // 独占存储，成功则 w0=0，失败则 w0=1
```

## 2. 独占监视器

ARM64 使用本地和全局独占监视器：

1. **LDXR**: 标记地址为"被监视"
2. **STXR**: 检查监视状态
   - 如果地址仍被监视，存储成功，返回 0
   - 如果地址被其他核心或异常修改，存储失败，返回 1

## 3. 原子操作模式

```asm
atomic_store:
1:
    ldxr x0, [x1]       // 独占加载
    stxr w2, x0, [x1]   // 独占存储
    cbnz w2, 1b         // 失败则重试
    ret
```

## 4. 内存屏障

在多核环境中，需要使用 DMB 指令：

```asm
dmb ish    // 内部共享域内存屏障
```

## 5. 常见错误

1. 在 LDXR 和 STXR 之间修改了加载的寄存器
2. 在 LDXR 和 STXR 之间调用了其他函数
3. 忘记检查 STXR 的返回值