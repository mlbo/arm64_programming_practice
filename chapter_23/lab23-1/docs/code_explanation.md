# 代码说明

## 核心函数解析

### 1. 检测 SVE 支持

```cpp
auto has_sve() -> bool {
    u64 id_aa64pfr0;
    asm volatile("mrs %0, ID_AA64PFR0_EL1" : "=r"(id_aa64pfr0));
    return ((id_aa64pfr0 >> 32) & 0xF) != 0;
}
```

在用户态使用系统调用：

```cpp
#include <sys/auxv.h>

auto has_sve_user() -> bool {
    return (getauxval(AT_HWCAP) & HWCAP_SVE) != 0;
}
```

### 2. 获取向量长度

```cpp
auto get_vector_length() -> u64 {
    u64 vl;
    asm volatile("rdvl %0, #1" : "=r"(vl));
    return vl;  // 返回字节数
}
```

### 3. Predicate 基本操作

```cpp
void predicate_demo() {
    asm volatile (
        "ptrue p0.b\n"          // 所有 byte lane 激活
        "ptrue p1.s, vl4\n"     // 4 个 word lane 激活
        "whilelo p2.s, xzr, x4\n" // 0 <= i < 4 的 lane 激活
    );
}
```

### 4. VLA 循环示例

```cpp
void add_sve(const float* a, const float* b, float* c, int n) {
    int i = 0;
    svbool_t pg = svptrue_b32();

    while (i < n) {
        svbool_t pg_i = svwhilelt_b32(i, n);
        svfloat32_t va = svld1_f32(pg_i, a + i);
        svfloat32_t vb = svld1_f32(pg_i, b + i);
        svfloat32_t vc = svadd_f32_z(pg_i, va, vb);
        svst1_f32(pg_i, c + i, vc);
        i += svcntw();
    }
}
```

## SVE ACLE Intrinsics

ARM 提供了 C 语言级别的 SVE intrinsics (ACLE - ARM C Language Extensions)：

### 类型命名规则

| 类型 | 说明 |
|------|------|
| `svbool_t` | Predicate 类型 |
| `svfloat32_t` | float32 向量 |
| `svint32_t` | int32 向量 |
| `svuint8_t` | uint8 向量 |

### 函数命名规则

```
sv<op>_<type>[_<mode>]

示例:
svld1_f32      // 加载 float32
svadd_f32_z    // 加法，zeroing 模式
svst1_f32      // 存储 float32
```

### Predicate 后缀

| 后缀 | 说明 |
|------|------|
| `_z` | Zeroing: 非激活 lane 写 0 |
| `_m` | Merging: 非激活 lane 保持原值 |
| `_x` | Don't care: 非激活 lane 未定义 |

## 汇编与 Intrinsics 对比

```asm
// 汇编
ld1w z0.s, p0/z, [x0]
add  z0.s, z0.s, z1.s
st1w z0.s, p0, [x1]
```

```cpp
// Intrinsics
svfloat32_t v = svld1_f32(pg, ptr);
v = svadd_f32_z(pg, v, other);
svst1_f32(pg, ptr, v);
```