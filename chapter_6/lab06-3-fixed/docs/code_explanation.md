# 代码说明

## my_atomic_write 函数

```asm
.section .data
.align 3
.global my_data
my_data:
    .dword 0x0

.section .text
.global my_atomic_write
my_atomic_write:
    adr x6, my_data
1:
    ldxr x2, [x6]      // 独占加载 my_data
    mov  x2, x0        // 设置新值为参数
    stxr w3, x2, [x6]  // 尝试存储
    cbnz w3, 1b        // 失败则重试
    mov x0, x2         // 返回写入的值
    ret
```

## 执行流程

1. 加载 my_data 地址到 x6
2. 独占加载 my_data 的值（被覆盖，实际未使用）
3. 设置 x2 为参数值（要写入的新值）
4. 独占存储 x2 到 my_data
5. 如果存储失败，重新尝试
6. 返回写入的值

## 改进版本

更清晰的实现：

```asm
atomic_write:
    // x0 = value to write
    adr x1, my_data
1:
    ldxr x2, [x1]
    stxr w3, x0, [x1]
    cbnz w3, 1b
    ret
```