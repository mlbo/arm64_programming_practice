# 知识点：LDR/STR 寻址模式

## 1. LDR 指令寻址模式

### 1.1 基础偏移寻址

```asm
ldr x0, [x1]           // 无偏移，读取 x1 地址的值
ldr x2, [x1, #8]       // 立即数偏移，读取 x1+8 地址的值
ldr x4, [x1, x3]       // 寄存器偏移，读取 x1+x3 地址的值
ldr x5, [x1, x3, lsl #3]  // 扩展寄存器偏移，读取 x1+(x3<<3) 地址的值
```

### 1.2 前变基寻址 (Pre-indexed)

```asm
ldr x6, [x1, #8]!      // x1 = x1 + 8，然后读取 x1 地址的值
```

**特点**: 先更新基地址，再访问内存。

### 1.3 后变基寻址 (Post-indexed)

```asm
ldr x7, [x1], #8       // 读取 x1 地址的值，然后 x1 = x1 + 8
```

**特点**: 先访问内存，再更新基地址。

## 2. 条件标志位

| 标志位 | 含义 | 设置条件 |
|--------|------|----------|
| N | Negative | 结果为负数 |
| Z | Zero | 结果为零 |
| C | Carry | 无符号溢出/进位 |
| V | Overflow | 有符号溢出 |

## 3. CMP 指令

```asm
cmp x1, x2    // 计算 x1 - x2，设置条件标志位
```

CMP 指令本质上是 SUBS 指令，但不保存结果，只影响标志位。

## 4. 条件后缀

| 后缀 | 含义 | 条件 |
|------|------|------|
| eq | Equal | Z=1 |
| ne | Not Equal | Z=0 |
| mi | Minus/Negative | N=1 |
| cs/hs | Carry Set/Unsigned Higher or Same | C=1 |
| cc/lo | Carry Clear/Unsigned Lower | C=0 |
| hi | Unsigned Higher | C=1 && Z=0 |
| ls | Unsigned Lower or Same | C=0 \|\| Z=1 |
| ge | Signed Greater or Equal | N=V |
| lt | Signed Less Than | N!=V |
| gt | Signed Greater Than | Z=0 && N=V |
| le | Signed Less or Equal | Z=1 \|\| N!=V |

## 5. BL 指令与返回地址

```asm
bl function    // 跳转到 function，并将返回地址保存到 x30 (LR)
```

**重要**: BL 指令会修改 x30 寄存器，如果函数中再次调用 BL，需要先保存 x30。