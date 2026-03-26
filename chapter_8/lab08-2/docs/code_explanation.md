# 代码说明

## main.c

```c
#include <stdio.h>

extern int compare_data(int a, int b);

int main() {
    int val;
    val = compare_data(5, 6);
    printf("big data: %d\n", val);
}
```

## compare.S

```asm
.section .text
.globl compare_data
compare_data:
    cmp x0, x1          // 比较 a 和 b
    csel x0, x0, x1, hi // 如果 a > b，返回 a，否则返回 b
    ret
```

## 执行流程

1. main() 调用 compare_data(5, 6)
2. x0 = 5, x1 = 6
3. cmp x0, x1 设置标志位 (5 < 6，所以 C=0)
4. csel x0, x0, x1, hi：条件不满足，x0 = x1 = 6
5. 返回 x0 = 6
6. main() 打印 "big data: 6"