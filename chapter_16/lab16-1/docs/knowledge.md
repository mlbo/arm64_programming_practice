# False Sharing 知识点

## 1. 缓存层次结构

现代处理器通常有三层缓存：
- **L1 Cache**: 最快最小 (32-64 KB)，每个核心独占
- **L2 Cache**: 中等速度 (256 KB - 1 MB)，每个核心或几个核心共享
- **LLC (L3)**: 最慢最大 (几 MB - 几十 MB)，所有核心共享

## 2. 缓存行 (Cache Line)

- 缓存以"行"为单位操作，通常为 **64 字节**
- 即使只访问 1 字节，也会加载整个 64 字节缓存行
- 这导致相邻变量可能在同一缓存行中

## 3. MESI 协议

四状态协议维护缓存一致性：

| 状态 | 含义 | 可执行操作 |
|------|------|-----------|
| M (Modified) | 已修改，数据与内存不一致 | 可读可写 |
| E (Exclusive) | 独占，数据与内存一致 | 可读可写 |
| S (Shared) | 共享，可能多个缓存有副本 | 只读 |
| I (Invalid) | 无效 | 需要从内存或其他缓存获取 |

## 4. False Sharing 成因

```
+------------------+  缓存行 64 字节
| count1 | count2 |  <- 两个变量在同一缓存行
+------------------+
    CPU0       CPU1
    写count1   写count2
       ↓          ↓
    缓存行在 CPU0 和 CPU1 之间来回传递
```

当 CPU0 写入 count1 时：
1. 发送 invalidate 消息给其他 CPU
2. CPU1 的缓存行变为 Invalid
3. CPU1 需要访问 count2 时，必须重新获取缓存行
4. 这个过程重复数百万次，导致严重性能下降

## 5. 解决方案

### 方案一：缓存行对齐
```cpp
struct CounterGood {
    alignas(64) u64 count1;  // 独占一个缓存行
    alignas(64) u64 count2;  // 独占另一个缓存行
};
```

### 方案二：填充字节
```cpp
struct CounterPadded {
    u64 count1;
    char padding[56];  // 填充到 64 字节
    u64 count2;
};
```

### 方案三：使用模板包装器
```cpp
template<typename T>
struct alignas(64) CacheLineAligned {
    T value;
};
```

## 6. 性能影响

False Sharing 可能导致：
- 性能下降 2-10 倍
- 总线流量增加
- 功耗增加

## 7. 检测工具

- **perf c2c**: Linux 性能分析工具，专门用于检测 False Sharing
- **Intel VTune**: 商业性能分析工具
- **Valgrind**: 使用 cachegrind 工具