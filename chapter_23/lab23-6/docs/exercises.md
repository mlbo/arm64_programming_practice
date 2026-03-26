# 练习题

## 基础练习

### 1. 实现 strlen

使用 FFR 实现 strlen 函数：

```cpp
size_t strlen_ffr(const char* s);
```

**提示**: 使用 `ldff1b` 和 `cmpeq` 查找 '\0'。

### 2. 实现 memchr

使用 FFR 实现 memchr（查找字符）：

```cpp
void* memchr_ffr(const void* s, int c, size_t n);
```

## 进阶练习

### 3. 安全数组求和

实现一个安全的数组求和函数，处理可能越界的数组：

```cpp
int safe_sum_ffr(const int* arr);
```

### 4. 字符串复制

使用 FFR 实现 strcpy：

```cpp
char* strcpy_ffr(char* dest, const char* src);
```

### 5. 性能对比

比较 FFR 版本与普通版本的性能：

```cpp
// 普通 strlen
size_t strlen_normal(const char* s);

// FFR strlen
size_t strlen_ffr(const char* s);
```

测试不同长度的字符串。

## 思考题

1. FFR 相比传统的边界检查有什么优势？
2. 为什么 FFR 会累积多次 ldff1 的失败信息？
3. 什么情况下应该使用 ldff1 而不是普通的 ld1？

## 参考资料

- ARM SVE Architecture Reference Manual
- "Programming with SVE" - ARM Learn