# Lab 08-2: C 调用汇编函数

## 实验目的

1. 学习 C 语言调用汇编函数
2. 理解 extern 声明
3. 掌握跨语言调用约定

## 核心代码

### C 主程序 (main.c)

```c
#include <stdio.h>

extern int compare_data(int a, int b);  // 声明汇编函数

int main() {
    int val = compare_data(5, 6);
    printf("big data: %d\n", val);
    return 0;
}
```

### 汇编函数 (compare.S)

```asm
.text
.globl compare_data
compare_data:
    cmp x0, x1
    csel x0, x0, x1, hi   // 返回较大值
    ret
```

## 构建与运行

```bash
make && ./main
```