# Lab10-1: 基础内联汇编

## 实验目的

学习 ARM64 GCC 内联汇编的基本语法，掌握如何在 C/C++ 代码中嵌入汇编指令。

## 实验内容

- 基本 `asm` 语法
- `volatile` 关键字的作用
- 简单的内存拷贝实现
- 使用内联汇编优化代码

## 知识点

### 内联汇编语法

```cpp
asm volatile (
    "汇编代码"
    : 输出操作数
    : 输入操作数
    : 破坏列表
);
```

### 示例：简单加法

```cpp
auto add(int a, int b) -> int {
    int result;
    asm volatile (
        "add %w0, %w1, %w2"
        : "=r" (result)     // 输出
        : "r" (a), "r" (b)  // 输入
    );
    return result;
}
```

## 运行方法

```bash
make && make run
```