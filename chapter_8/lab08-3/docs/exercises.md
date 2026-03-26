# 练习题

## 练习 1: 调用数学函数

在汇编中调用 C 标准库的 abs 函数：

```asm
mov x0, #-10
bl abs       // 返回 10
```

## 练习 2: 调用字符串函数

编写汇编程序调用 strlen：

```asm
ldr x0, =my_string
bl strlen    // 返回字符串长度
```

## 练习 3: 多参数调用

编写汇编程序调用 printf 输出多个值：

```c
printf("a=%d, b=%d, sum=%d\n", a, b, a+b);
```

## 练习 4: 回调函数

在汇编中调用一个接受函数指针的 C 函数：

```c
int apply(int (*fn)(int, int), int a, int b);
```