# 知识点: LDR 加载 Label

## PC 相对加载

```asm
ldr x0, label
```

- 从 `PC + offset` 地址加载
- 偏移量范围: ±1MB

## 伪指令 `=`

```asm
ldr x0, =value
ldr x0, =label
```

**汇编器处理:**

1. 如果 `value` 可以用 `mov` 指令表示，生成 `mov` 指令
2. 否则在附近创建 literal pool，加载地址

## 示例

```asm
ldr x6, MY_LABEL      // 加载 0x20
ldr x7, =MY_LABEL     // 加载 MY_LABEL 的地址

// 等价于:
ldr x6, [PC, #offset]  // 从 literal pool 加载值
ldr x7, [PC, #offset2] // 从 literal pool 加载地址
```

## Literal Pool

汇编器会在代码附近放置一个数据区域，存储无法用立即数表示的常量:

```asm
.text
    ldr x0, =0x1234567890ABCDEF
    // ...
.ltorg  // literal pool 位置
```