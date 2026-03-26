# 练习题

## 基础练习

### 1. RGBA/BGRA 转换

修改代码实现 4 通道 RGBA 到 BGRA 的转换。

**提示**: 使用 `svld4_u8` 和 `svst4_u8`。

### 2. 分离通道

使用 SVE 实现 RGB 图像的通道分离：

```cpp
void split_rgb_sve(const u8* rgb,
                   u8* r, u8* g, u8* b,
                   usize pixels);
```

## 进阶练习

### 3. 图像缩放

使用 SVE 实现图像双线性插值缩放。

### 4. 颜色空间转换

使用 SVE 实现 RGB 到 YUV 的颜色空间转换：

```
Y = 0.299R + 0.587G + 0.114B
U = -0.147R - 0.289G + 0.436B
V = 0.615R - 0.515G - 0.100B
```

### 5. 性能基准

编写基准测试，比较：
- C 标量版本
- NEON 版本
- SVE Intrinsics 版本
- SVE 汇编版本

记录并分析不同向量长度下的性能差异。

## 思考题

1. SVE 的交错加载与 NEON 有什么本质区别？
2. 为什么 `incb x3, all, mul #3` 要乘以 3？
3. 如何确定 SVE 交错操作的最优向量长度？