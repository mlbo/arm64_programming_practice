# SVE 基础知识

## SVE 概述

SVE (Scalable Vector Extension) 是 ARMv8-A 架构的可伸缩向量扩展，主要特点：

1. **向量长度可变** - 硬件实现可选择 128-bit 到 2048-bit
2. **Predicate 控制** - 16 个 Predicate 寄存器实现条件执行
3. **VLA 编程** - 同一代码可在不同向量长度的处理器上运行

### 与 NEON 的区别

| 特性 | NEON | SVE |
|------|------|-----|
| 向量长度 | 固定 128-bit | 可变 128-2048 bit |
| Predicate | 无 | P0-P15 |
| 编程模型 | 固定长度 | VLA (Vector Length Agnostic) |
| 控制流 | 分支 | Predicate 条件执行 |

## SVE 寄存器

### 向量寄存器 Z0-Z31

每个 Z 寄存器是 V 寄存器的扩展：
- 低 128-bit 与 NEON V 寄存器重叠
- 高位部分仅 SVE 可访问

```
         SVE 向量长度 (128-2048 bits)
    +------------------------------------+
    |           Z0 [VL-1:0]              |
    +------------------------------------+
          |-----------|
          V0 [127:0] (NEON)
```

### Predicate 寄存器 P0-P15

每个 P 寄存器控制向量元素的激活状态：
- 每 bit 控制一个向量 lane
- 长度 = 向量长度 / 8 (字节粒度)

```
向量长度 256-bit 时:
P0: [b31][b30][b29]...[b2][b1][b0]
      |    |    |      |   |   |
     Z0  Z0   Z0     Z0  Z0  Z0
    [31] [30] [29]   [2] [1] [0]
```

## 关键 SVE 指令

### 向量长度查询

```asm
rdvl x0, #1      // 读取向量长度（字节）
rdvl x0, #8      // 读取向量长度 / 8（元素数，对于 64-bit 元素）
```

### Predicate 设置

```asm
ptrue p0.b       // 设置所有 byte lane 为激活
ptrue p0.s, vl4  // 设置 4 个 word lane 为激活
whilelo p0.s, x0, x1  // x0 <= i < x1 的 lane 设为激活
```

### 条件加载

```asm
ld1b z0.b, p0/z, [x0]  // 条件加载字节
st1b z0.b, p0, [x0]    // 条件存储字节
```

### Predicate 逻辑

```asm
and p0.b, p1/z, p2.b, p3.b  // p0 = p2 & p3 (under p1)
orr p0.b, p1/z, p2.b, p3.b  // p0 = p2 | p3
not p0.b, p1/z, p2.b        // p0 = ~p2
```

## VLA 编程模式

### 传统 SIMD (固定长度)

```cpp
// 假设向量长度为 128-bit (16 字节)
for (int i = 0; i < n; i += 16) {
    // 处理 16 字节
}
```

### SVE VLA 模式

```cpp
// 不假设向量长度
int i = 0;
while (i < n) {
    // 设置 predicate: i <= lane < n
    svbool_t pg = svwhilelt_b32(i, n);

    // 加载并处理
    svfloat32_t vec = svld1_f32(pg, src + i);
    // ... 处理 ...
    svst1_f32(pg, dst + i, vec);

    // 增加向量长度
    i += svlen_f32(vec);
}
```

## SVE 检测

在用户态检测 SVE 支持：

```cpp
#include <sys/auxv.h>
#include <asm/hwcap.h>

bool has_sve() {
    return (getauxval(AT_HWCAP) & HWCAP_SVE) != 0;
}
```