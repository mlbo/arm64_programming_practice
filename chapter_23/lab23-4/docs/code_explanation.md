# 代码说明

## 核心函数解析

### strcmp_sve 汇编实现

```asm
.global strcmp_sve
strcmp_sve:
    ptrue   p5.b
    setffr

    mov     x5, #0

l_loop:
    ldff1b  z0.b, p5/z, [x0, x5]
    ldff1b  z1.b, p5/z, [x1, x5]
    rdffrs  p7.b, p5/z
    b.nlast l_fault

    incb    x5
    cmpeq   p0.b, p5/z, z0.b, #0
    cmpne   p1.b, p5/z, z0.b, z1.b
l_test:
    orrs    p4.b, p5/z, p0.b, p1.b
    b.none  l_loop

l_return:
    brkb    p4.b, p5/z, p4.b
    lasta   w0, p4, z0.b
    lasta   w1, p4, z1.b
    sub     w0, w0, w1
    ret

l_fault:
    incp    x5, p7.b
    setffr
    cmpeq   p0.b, p7/z, z0.b, #0
    cmpne   p1.b, p7/z, z0.b, z1.b
    b       l_test
```

## 执行流程详解

### 1. 初始化

```asm
ptrue   p5.b        // 设置全 1 predicate
setffr              // 初始化 FFR
mov     x5, #0      // 偏移量清零
```

### 2. 主循环

```asm
ldff1b  z0.b, p5/z, [x0, x5]   // 推测加载 s1[x5...x5+VL]
ldff1b  z1.b, p5/z, [x1, x5]   // 推测加载 s2[x5...x5+VL]
```

`ldff1b` 是 First-Fault 加载：
- 不会因越界而异常
- FFR 记录哪些位置成功加载

### 3. FFR 检查

```asm
rdffrs  p7.b, p5/z    // 读取 FFR 到 p7
b.nlast l_fault       // 如果不是最后部分，处理错误
```

### 4. 字符比较

```asm
cmpeq   p0.b, p5/z, z0.b, #0     // 检测 '\0'
cmpne   p1.b, p5/z, z0.b, z1.b   // 检测字符差异
orrs    p4.b, p5/z, p0.b, p1.b   // 合并条件
b.none  l_loop                   // 都不满足则继续
```

### 5. 结果提取

```asm
brkb    p4.b, p5/z, p4.b   // 找到第一个匹配位置
lasta   w0, p4, z0.b       // 提取 s1 的字符
lasta   w1, p4, z1.b       // 提取 s2 的字符
sub     w0, w0, w1         // 返回差值
```

## 为什么需要 FFR？

普通 `ld1b` 在访问无效内存时会触发异常。`ldff1b` 配合 FFR 可以：
1. 安全地加载"越界"数据
2. 知道哪些位置是有效的
3. 避免处理字符串长度

这是 SVE 处理变长数据的独特优势。