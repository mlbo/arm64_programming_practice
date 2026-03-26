# 练习题

## 练习 1: 手动建立栈帧

编写一个函数，使用 STP/LDP 建立栈帧，并在栈上存储两个局部变量。

## 练习 2: 栈溢出

分析以下代码的问题：

```c
void recursive(int n) {
    int local = n;
    if (n > 0) recursive(n - 1);
}
```

如果 n 很大，会发生什么？

## 练习 3: 栈对齐

为什么 ARM64 要求栈 16 字节对齐？如果不对齐会发生什么？

## 练习 4: 叶子函数优化

以下函数是否需要保存 x30？为什么？

```asm
leaf_func:
    add x0, x0, x1
    ret
```

## 练习 5: 反汇编分析

使用 `objdump -d` 反汇编一个简单的 C 函数，分析其栈帧布局。

## 练习 6: 内联汇编

使用 C++ 内联汇编实现一个带栈帧保护的函数：

```cpp
int safe_function(int a, int b) {
    int result;
    // 使用内联汇编：
    // 1. 建立栈帧
    // 2. 计算 a + b
    // 3. 恢复栈帧
    return result;
}
```