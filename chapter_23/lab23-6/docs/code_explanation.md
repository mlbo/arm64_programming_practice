# 代码说明

## FFR 示例代码解析

### 1. 基本 FFR 使用

```cpp
void ffr_basic_demo() {
    asm volatile (
        "ptrue p0.b\n"       // 设置全 1 predicate
        "setffr\n"           // 初始化 FFR

        // 推测性加载
        "ldff1b z0.b, p0/z, [x0]\n"

        // 检查 FFR
        "rdffrs p1.b, p0/z\n"
        "b.nlast 1f\n"       // 如果有失败，跳转

        // 所有加载成功
        // ...

        "1:\n"
        // 处理部分成功
        // ...
    );
}
```

### 2. 处理未知长度数组

```cpp
void process_unknown_length(const u8* data, u8* result) {
    asm volatile (
        "ptrue p0.b\n"
        "setffr\n"
        "mov x2, #0\n"

        "1:\n"
        "ldff1b z0.b, p0/z, [%[data], x2]\n"
        "rdffrs p1.b, p0/z\n"
        "b.nlast 2f\n"

        // 处理数据（所有 lane 有效）
        "add z0.b, z0.b, #1\n"  // 示例：每个元素加 1
        "st1b z0.b, p0, [%[result], x2]\n"

        "incb x2\n"
        "b 1b\n"

        "2:\n"
        // 处理边界情况
        "incp x2, p1.b\n"       // 跳过无效位置
        // ...
        :
        : [data] "r" (data), [result] "r" (result)
        : "memory", "x2", "p0", "p1", "z0"
    );
}
```

### 3. FFR 累积效应

```cpp
void ffr_cumulative_demo() {
    // FFR 会累积多次 ldff1 的失败信息
    asm volatile (
        "ptrue p0.b\n"
        "setffr\n"

        // 第一次加载：FFR 记录第一组失败位置
        "ldff1b z0.b, p0/z, [x0]\n"

        // 第二次加载：FFR 累积更多失败位置
        "ldff1b z1.b, p0/z, [x1]\n"

        // FFR 现在包含两次加载的所有失败信息
        "rdffrs p1.b, p0/z\n"

        // 重置 FFR 以便下次使用
        "setffr\n"
    );
}
```

## 关键指令详解

### b.nlast / b.last

FFR 条件分支：

```asm
b.nlast label    // FFR 不是 "last" (有有效 lane) 时跳转
b.last label     // FFR 是 "last" (无有效 lane) 时跳转
```

### incp

根据 predicate 增加指针：

```asm
incp x0, p0.b    // x0 += p0 中激活 lane 的数量
```

用于跳过已处理的有效元素。

## 调试 FFR

### 打印 FFR 状态

```cpp
void print_ffr_status() {
    u8 ffr_bytes[32];  // 足够大的缓冲区

    asm volatile (
        "ptrue p0.b\n"
        "rdffrs p0.b, p0/z\n"
        "st1b z0.b, p0, [%[buf]]\n"
        :
        : [buf] "r" (ffr_bytes)
        : "memory", "p0", "z0"
    );

    // 打印 FFR 状态
    for (int i = 0; i < 32; ++i) {
        printf("%02x ", ffr_bytes[i]);
    }
    printf("\n");
}
```