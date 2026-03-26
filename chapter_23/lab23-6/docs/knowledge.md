# SVE FFR 知识

## First Fault Register (FFR) 概述

FFR 是 SVE 的特殊寄存器，用于记录推测性内存访问的成功/失败状态。

### 为什么需要 FFR？

传统 SIMD 的问题：
- 访问越界内存会触发异常
- 必须预先知道数据长度
- 处理边界条件复杂

SVE 的解决方案：
- 使用 First-Fault 加载指令
- FFR 记录哪些位置访问失败
- 程序可以安全处理边界

## FFR 工作原理

### 1. 初始化

```asm
setffr              // 将 FFR 初始化为全 1
```

### 2. First-Fault 加载

```asm
ldff1b z0.b, p0/z, [x0]    // 推测性加载
```

- 正常位置：数据加载成功，FFR 对应位保持 1
- 越界位置：加载失败但不异常，FFR 对应位清零

### 3. 检查 FFR

```asm
rdffrs p0.b, p0/z          // 读取 FFR 到 predicate
b.nlast label              // 如果不是最后部分，跳转
```

## FFR 指令详解

### setffr

初始化 FFR，将所有位设为 1：

```asm
setffr
```

### ldff1b / ldff1w / ldff1d

First-Fault 加载指令：

```asm
ldff1b z0.b, p0/z, [x0]    // 字节粒度
ldff1w z0.s, p0/z, [x0]    // 字粒度
ldff1d z0.d, p0/z, [x0]    // 双字粒度
```

特点：
- 访问有效内存 → 正常加载
- 访问无效内存 → 不异常，FFR 记录失败

### rdffrs / rdffr

读取 FFR 状态：

```asm
rdffrs p0.b, p0/z    // 读取 FFR（保留非激活 lane）
rdffr  p0.b, p0/z    // 读取 FFR（清除非激活 lane）
```

### wrffr

写入 FFR（高级用法）：

```asm
wrffr p0.b           // 将 predicate 写入 FFR
```

## FFR 使用模式

### 处理未知长度数组

```asm
process_array:
    ptrue   p0.b
    setffr
    mov     x2, #0

1:
    ldff1b  z0.b, p0/z, [x0, x2]
    rdffrs  p1.b, p0/z
    b.nlast 2f

    // 处理数据
    // ...

    incb    x2
    b       1b

2:
    // 处理边界情况
    // ...
    ret
```

### 安全搜索字符串

```asm
find_char:
    ptrue   p0.b
    setffr
    mov     x2, #0

1:
    ldff1b  z0.b, p0/z, [x0, x2]
    rdffrs  p1.b, p0/z

    cmpeq   p2.b, p1/z, z0.b, #target_char
    b.any   found

    incb    x2
    b       1b

found:
    // 找到目标字符
    // ...
    ret
```

## FFR vs 普通加载

| 特性 | ld1b | ldff1b |
|------|------|--------|
| 越界访问 | 触发异常 | 不异常，更新 FFR |
| 适用场景 | 已知长度 | 未知长度 |
| 性能 | 略快 | 略慢（需检查 FFR）|

## 注意事项

1. **FFR 必须初始化**：使用前必须 `setffr`
2. **FFR 是累积的**：多次 ldff1 会累积失败信息
3. **条件分支**：`b.nlast` / `b.last` 用于 FFR 条件分支
4. **重置 FFR**：处理完边界后应 `setffr` 重置

## 实际应用

- `strlen`: 查找 '\0' 但不知道长度
- `strcmp`: 比较两个字符串
- `memcpy`: 复制未知长度数据
- 搜索算法：线性搜索等