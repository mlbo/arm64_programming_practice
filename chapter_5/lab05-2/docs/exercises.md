# 练习题

## 练习 1: 实现最大值函数

使用 CSEL 指令实现：

```cpp
inline auto max_value(long a, long b) -> long {
    // 返回 a 和 b 中较大的值
}
```

## 练习 2: 实现三目运算符

将以下 C 代码转换为汇编：

```c
int select(int a, int b, int c) {
    return (a > b) ? b : c;
}
```

## 练习 3: 实现绝对值

使用 CNEG 指令实现：

```cpp
inline auto abs_value(long x) -> long {
    // 返回 x 的绝对值
}
```

## 练习 4: 条件选择链

实现一个函数，根据输入值返回不同结果：

- 如果 x > 100，返回 x - 100
- 如果 50 <= x <= 100，返回 0
- 如果 x < 50，返回 50 - x