# 练习题

## 基础练习

### 1. strncmp 实现

修改代码实现 strncmp（比较前 n 个字符）：

```cpp
int strncmp_sve(const char* s1, const char* s2, size_t n);
```

### 2. strlen 实现

使用 SVE 实现 strlen：

```cpp
size_t strlen_sve(const char* s);
```

**提示**: 只需检测 '\0'，不需要比较两个字符串。

## 进阶练习

### 3. strchr 实现

使用 SVE 实现 strchr（查找字符）：

```cpp
char* strchr_sve(const char* s, int c);
```

### 4. 性能对比

编写基准测试比较：
- glibc strcmp
- SVE strcmp
- 手写标量版本

测试不同长度的字符串。

### 5. 处理特殊字符

修改 strcmp 使其忽略大小写：

```cpp
int strcasecmp_sve(const char* s1, const char* s2);
```

## 思考题

1. 为什么 strcmp 需要 FFR？普通的加载指令有什么问题？
2. `lasta` 指令的作用是什么？
3. 如何优化 strcmp 对短字符串的处理？