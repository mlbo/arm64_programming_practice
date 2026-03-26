# 知识点: Label 访问

## LDR 伪指令

`ldr x0, =value` 是汇编器伪指令:

1. 如果 `value` 能用 `mov` 表示，生成 `mov` 指令
2. 否则在 literal pool 中存储值，生成 `ldr` 指令

## PC 相对加载

```asm
ldr x0, label    // 从 PC+offset 地址加载
```

- 偏移量范围: ±1MB
- 常用于加载局部数据

## Label vs =Label

| 指令 | 结果 |
|------|------|
| `ldr x0, label` | 加载 label 地址处的值 |
| `ldr x0, =label` | 加载 label 的地址 |

## 数据定义

```asm
.string "text"   // 字符串
.word 0x1234     // 32 位数据
.quad 0x12345678 // 64 位数据
.align 3         // 8 字节对齐
```