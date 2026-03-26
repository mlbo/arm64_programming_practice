# 代码说明

## 查找表实现

```asm
.align 3
.global func_addr
func_addr:
    .quad 0x800800    // 函数地址表
    .quad 0x800860
    .quad 0x800880

.align 3
.global func_string
func_string:
    .asciz "func_a"   // 函数名表
    .asciz "func_b"
    .asciz "func_c"

.global func_num_syms
func_num_syms:
    .quad 3           // 函数数量
```

## 宏定义使用

```asm
.macro add_func add, a, b
    mov x0, \a
    mov x1, \b
    bl add_\()\add
.endm

.global macro_test_1
macro_test_1:
    mov x9, x30
    add_func 1, x0, x1    // 展开为调用 add_1
    mov x30, x9
    ret
```

## C 中的查找表使用

```c
extern unsigned long func_addr[];
extern unsigned long func_num_syms;
extern char func_string[];

int print_func_name(unsigned long addr) {
    for (int i = 0; i < func_num_syms; i++) {
        if (addr == func_addr[i]) {
            // 找到函数名并打印
        }
    }
    return 0;
}
```