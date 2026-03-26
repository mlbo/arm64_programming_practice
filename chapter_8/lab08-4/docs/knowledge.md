# 知识点：汇编器伪操作

## 1. 数据定义伪操作

### .quad (64 位)

```asm
my_data:
    .quad 0x123456789ABCDEF0
    .quad 100, 200, 300
```

### .word (32 位)

```asm
my_word:
    .word 0x12345678
```

### .byte (8 位)

```asm
my_byte:
    .byte 0x12, 0x34, 0x56
```

### .asciz (空结尾字符串)

```asm
my_string:
    .asciz "Hello, ARM64!"
```

## 2. 对齐伪操作

```asm
.align 3    // 8 字节对齐 (2^3 = 8)
.align 4    // 16 字节对齐
```

## 3. 宏定义

```asm
.macro add_func add, a, b
    mov x0, \a
    mov x1, \b
    bl add_\()\add
.endm

// 使用
add_func 1, x0, x1
```

## 4. 符号导出

```asm
.global my_function    // 导出符号
.globl my_data         // 同 .global
```

## 5. 段定义

```asm
.section .text    // 代码段
.section .data    // 数据段
.section .bss     // 未初始化数据段
```