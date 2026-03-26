# SVE strcmp 知识

## strcmp 的挑战

实现 strcmp 的难点在于：
1. 不知道字符串长度
2. 需要检测空字符 '\0'
3. 需要比较对应字符的差异

### 标量实现

```cpp
int strcmp_c(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}
```

## SVE 解决方案

### 问题：如何检测字符串结束？

SVE 提供了 FFR (First Fault Register)：
- 记录首次访存失败的位置
- 允许"推测性"加载越过合法边界

### ldff1b 指令

```asm
ldff1b z0.b, p0/z, [x0]
```

加载字节向量，但不会因为越界访问而异常。越界位置通过 FFR 报告。

### FFR 操作流程

```asm
setffr              // 初始化 FFR
ldff1b z0.b, p0/z, [x0]  // 推测性加载
rdffrs p0.b, p0/z        // 读取 FFR 到 predicate
b.nlast label            // 如果不是最后部分，跳转
```

## strcmp_sve 实现详解

### 算法流程

1. 设置 FFR
2. 加载两个字符串的向量
3. 检查 FFR 状态（是否有越界）
4. 比较字符和检测空字符
5. 找到第一个差异或字符串结束
6. 返回差值

### 汇编代码

```asm
strcmp_sve:
    ptrue   p5.b            // 全 1 predicate
    setffr                  // 初始化 FFR

    mov     x5, #0          // 偏移量

l_loop:
    ldff1b  z0.b, p5/z, [x0, x5]   // 推测性加载 s1
    ldff1b  z1.b, p5/z, [x1, x5]   // 推测性加载 s2
    rdffrs  p7.b, p5/z             // 检查 FFR
    b.nlast l_fault                // 有越界则处理

    incb    x5                     // 偏移量 += VL
    cmpeq   p0.b, p5/z, z0.b, #0   // 检测 '\0'
    cmpne   p1.b, p5/z, z0.b, z1.b // 检测差异
l_test:
    orrs    p4.b, p5/z, p0.b, p1.b // '\0' 或差异
    b.none  l_loop                 // 都没有则继续

l_return:
    brkb    p4.b, p5/z, p4.b       // 找到第一个匹配
    lasta   w0, p4, z0.b           // 提取字符
    lasta   w1, p4, z1.b
    sub     w0, w0, w1             // 返回差值
    ret

l_fault:
    incp    x5, p7.b               // 跳过无效位置
    setffr                          // 重置 FFR
    cmpeq   p0.b, p7/z, z0.b, #0
    cmpne   p1.b, p7/z, z0.b, z1.b
    b       l_test
```

## 关键指令

| 指令 | 功能 |
|------|------|
| `setffr` | 初始化 FFR 为全 1 |
| `ldff1b` | 推测性加载字节 |
| `rdffrs` | 读取 FFR 状态 |
| `b.nlast` | FFR 不是最后部分时分支 |
| `brkb` | Predicate 断点 |
| `lasta` | 提取最后一个激活元素 |

## 与标准库 strcmp 对比

| 特性 | glibc strcmp | SVE strcmp |
|------|-------------|------------|
| 优化 | 多种策略 | 单一 VLA 算法 |
| 短字符串 | 特殊处理 | 统一处理 |
| 长字符串 | SSE/AVX | SVE |
| 可移植 | 是 | 仅 SVE 平台 |