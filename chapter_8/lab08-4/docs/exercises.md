# 练习题

## 练习 1: 创建函数指针表

创建一个函数指针表，包含 4 个数学运算函数（加、减、乘、除）。

## 练习 2: 实现动态调度

使用查找表实现一个简单的函数调度器：

```c
void dispatch(int func_id, int a, int b);
```

## 练习 3: 字符串表

创建一个字符串表，实现根据索引获取字符串的功能。

## 练习 4: 结构体对齐

在汇编中模拟 C 结构体：

```c
struct Person {
    char name[32];
    int age;
    long id;
};
```

## 练习 5: 链表节点

在汇编中定义链表节点结构：

```asm
.struct 0
node_next:
    .quad 0
.struct 8
node_data:
    .quad 0
.struct 16
node_size:
```