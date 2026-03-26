# 练习题

## 练习 1: 实现加法

编写汇编函数实现加法：

```c
extern int my_add(int a, int b);
```

## 练习 2: 实现最大值

编写汇编函数返回三个数中的最大值：

```c
extern int max_of_three(int a, int b, int c);
```

## 练习 3: 实现字符串长度

编写汇编函数计算字符串长度：

```c
extern int my_strlen(const char *str);
```

## 练习 4: 实现内存拷贝

编写汇编函数实现内存拷贝：

```c
extern void my_memcpy(void *dst, const void *src, int n);
```

## 练习 5: 实现原子操作

使用 LDXR/STXR 实现原子加法：

```c
extern int atomic_add(int *ptr, int value);
```