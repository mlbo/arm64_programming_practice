# 知识点: memcpy 实现

## 基本原理

内存拷贝的核心是:
1. 从源地址读取数据
2. 写入目标地址
3. 移动指针
4. 重复直到完成

## 后变基优化

使用后变基可以合并加载/存储和指针移动:

```asm
// 不使用后变基
loop:
    ldr    x4, [x1]
    add    x1, x1, #8
    str    x4, [x2]
    add    x2, x2, #8
    ...

// 使用后变基 (更高效)
loop:
    ldr    x4, [x1], #8
    str    x4, [x2], #8
    ...
```

## 性能考虑

1. **对齐**: 确保地址 8 字节对齐
2. **循环展开**: 减少分支开销
3. **预取**: 使用 `prfm` 指令

## 通用 memcpy

```asm
// void memcpy(void* dst, void* src, size_t n)
.global memcpy
memcpy:
    cmp    x2, #0
    b.eq   2f
1:
    ldrb   w3, [x1], #1
    strb   w3, [x0], #1
    subs   x2, x2, #1
    b.ne   1b
2:
    ret
```