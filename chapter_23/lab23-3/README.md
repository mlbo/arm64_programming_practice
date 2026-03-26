# Lab 23-3: SVE memcpy 实现

## 实验目标

- 使用 SVE 实现高性能内存拷贝
- 理解 SVE 向量内存操作的优化
- 对比不同数据粒度的拷贝效率

## 实验内容

本实验演示如何使用 SVE 指令实现高效的 memcpy 函数。

### 关键技术点

1. **字节粒度拷贝**: `ld1b`, `st1b`
2. **字粒度拷贝**: `ld1w`, `st1w`
3. **Predicate 循环控制**: `whilelt` 指令

## 构建与运行

```bash
make && ./sve_memcpy_test
```

## 预期输出

```
1-byte granularity memcpy test: PASSED
4-byte granularity memcpy test: PASSED
Performance: X.XX GB/s
```

## 参考文献

- ARM SVE Optimization Guide
- "Efficient Memory Copy with SVE"