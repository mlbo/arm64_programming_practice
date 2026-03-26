# 知识点: STR 前/后变基

## STR 前变基

```asm
str x0, [x1, #8]!
```

**执行过程:**
1. `x1 = x1 + 8`
2. 将 `x0` 存储到新 `x1` 地址

## STR 后变基

```asm
str x0, [x1], #8
```

**执行过程:**
1. 将 `x0` 存储到 `x1` 地址
2. `x1 = x1 + 8`

## 内存填充示例

```asm
// 填充 64 字节为 0xFF
mov    x1, buffer_addr
mov    x2, #8
ldr    x3, =0xFFFFFFFFFFFFFFFF
fill_loop:
    str    x3, [x1], #8
    subs   x2, x2, #1
    b.ne   fill_loop
```

## 数组写入示例

```asm
// 写入数组元素
mov    x1, array_addr
mov    x2, #0
mov    x3, #10
write_loop:
    str    x2, [x1], #8
    add    x2, x2, #1
    subs   x3, x3, #1
    b.ne   write_loop
```