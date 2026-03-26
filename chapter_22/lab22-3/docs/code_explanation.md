# 代码说明

## 文件结构

```
lab22-3/
├── include/
│   └── auto_vectorization.hpp
├── src/
│   ├── auto_vec.cpp       # 自动向量化测试
│   ├── neon_manual.cpp    # 手动 NEON 版本
│   └── main.cpp           # 测试主程序
└── Makefile
```

## 测试用例

### 1. 简单数组加法

```cpp
// 标量版本 - 编译器容易向量化
void add_scalar(const float* a, const float* b, float* c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

// 手动 NEON 版本
void add_neon(const float* a, const float* b, float* c, int n) {
    int i = 0;
    for (; i + 3 < n; i += 4) {
        float32x4_t va = vld1q_f32(a + i);
        float32x4_t vb = vld1q_f32(b + i);
        float32x4_t vc = vaddq_f32(va, vb);
        vst1q_f32(c + i, vc);
    }
    for (; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}
```

### 2. RGB/BGR 转换对比

```cpp
// 可能无法向量化的版本（交错数据）
void rgb_to_bgr_not_vectorizable(const u8* src, u8* dst, int pixels) {
    for (int i = 0; i < pixels; ++i) {
        dst[i*3 + 0] = src[i*3 + 2];
        dst[i*3 + 1] = src[i*3 + 1];
        dst[i*3 + 2] = src[i*3 + 0];
    }
}

// 手动 NEON 使用交错加载指令
void rgb_to_bgr_neon(const u8* src, u8* dst, int pixels) {
    // 使用 vld3q_u8 交错加载
}
```

### 3. 使用 restrict 关键字

```cpp
// 无 restrict - 编译器保守处理
void scale_no_restrict(float* a, float* b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] * 2;
    }
}

// 有 restrict - 编译器积极向量化
void scale_with_restrict(float* __restrict__ a,
                          float* __restrict__ b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = b[i] * 2;
    }
}
```

## 编译器向量化报告示例

```
test.cpp:10:5: note: loop vectorized
test.cpp:20:5: note: not vectorized: data dependence
test.cpp:30:5: note: not vectorized: control flow
```

## 性能对比要点

| 场景 | 自动向量化 | 手动 NEON |
|------|-----------|-----------|
| 简单循环 | 相当 | 相当 |
| 复杂内存访问 | 可能失败 | 可控 |
| 交错数据 | 较难处理 | 可使用特殊指令 |
| 可维护性 | 好 | 较差 |