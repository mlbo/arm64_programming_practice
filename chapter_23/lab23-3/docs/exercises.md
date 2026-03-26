# 练习题

## 基础练习

### 1. 8 字节粒度 memcpy

实现 8 字节粒度 (double) 的 SVE memcpy。

**提示**: 使用 `ld1d`, `st1d`, `whilelt p0.d`, `incd`。

### 2. memset 实现

使用 SVE 实现 memset 函数：

```cpp
void sve_memset(void* dst, u8 value, size_t n);
```

**提示**: 使用 `svdup_u8` 创建常量向量。

## 进阶练习

### 3. 带对齐检查的 memcpy

实现一个智能 memcpy：
- 检查地址对齐
- 选择最优的粒度 (1B, 4B, 8B)

### 4. 非时序存储

研究如何使用非时序存储指令优化大块内存拷贝。

### 5. 性能对比

编写基准测试比较：
- glibc memcpy
- SVE 1B 版本
- SVE 4B 版本
- 手写汇编版本

绘制性能曲线图。

## 思考题

1. 为什么 4 字节粒度版本需要数据对齐？
2. SVE memcpy 相比 NEON memcpy 有什么优势？
3. 如何处理源和目标地址重叠的情况？