# 知识点: 前/后变基寻址

## 前变基 (Pre-indexed)

```asm
ldr x0, [x1, #8]!
```

**执行顺序:**
1. `x1 = x1 + 8`
2. 从新 `x1` 地址加载数据到 `x0`

**等价于:**
```asm
add x1, x1, #8
ldr x0, [x1]
```

## 后变基 (Post-indexed)

```asm
ldr x0, [x1], #8
```

**执行顺序:**
1. 从 `x1` 地址加载数据到 `x0`
2. `x1 = x1 + 8`

**等价于:**
```asm
ldr x0, [x1]
add x1, x1, #8
```

## 对比

| 模式 | 更新时机 | 指令特征 |
|------|----------|----------|
| 前变基 | 加载前 | 有 `!` 后缀 |
| 后变基 | 加载后 | 偏移量在 `]` 外 |

## 应用示例

### 数组遍历

```asm
mov x1, array_addr
mov x2, #10          // 10 elements
loop:
    ldr x0, [x1], #8  // 加载并移动到下一个
    // 处理 x0
    subs x2, x2, #1
    b.ne loop
```

### 内存清零

```asm
mov x1, dest_addr
mov x2, #size
zero_loop:
    str xzr, [x1], #8  // 存储 0 并移动
    subs x2, x2, #8
    b.ne zero_loop
```