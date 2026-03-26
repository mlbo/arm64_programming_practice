# Lab 08-3: 汇编调用 C 函数

## 实验目的

1. 学习汇编调用 C 函数
2. 理解 BL 指令的使用
3. 掌握参数传递约定

## 核心代码

### 汇编主程序 (main.S)

```asm
main:
    stp x29, x30, [sp, -16]!
    mov x0, #6           // 参数 a
    mov x1, #5           // 参数 b
    bl compare_data      // 调用 C 函数
    // x0 = 返回值
    ldp x29, x30, [sp], 16
    ret
```

### C 函数 (compare.c)

```c
int compare_data(int a, int b) {
    return (a >= b) ? a : b;
}
```

## 构建与运行

```bash
make && ./main
```