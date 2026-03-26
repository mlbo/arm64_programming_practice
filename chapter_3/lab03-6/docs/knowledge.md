# 知识点: memset 实现

## STP/LDP 指令

**STP (Store Pair):**
```asm
stp x1, x2, [x0]       // 存储 x1, x2 到 x0, x0+8
stp x1, x2, [x0, #16]! // 前变基
stp x1, x2, [x0], #16  // 后变基
```

**LDP (Load Pair):**
```asm
ldp x1, x2, [x0]       // 加载到 x1, x2
```

## 性能优化

### 对齐访问

对齐的内存访问更快:
- 16 字节对齐: STP 可以一次完成
- 非对齐: 需要额外处理

### 循环展开

```asm
// 未展开
loop:
    stp x1, x1, [x0], #16
    subs x2, x2, #16
    b.ne loop

// 展开 4 次
loop:
    stp x1, x1, [x0], #16
    stp x1, x1, [x0], #16
    stp x1, x1, [x0], #16
    stp x1, x1, [x0], #16
    subs x2, x2, #64
    b.ne loop
```

## 标准 memset

```cpp
void* memset(void* s, int c, size_t n) {
    unsigned char* p = static_cast<unsigned char*>(s);
    while (n--) {
        *p++ = static_cast<unsigned char>(c);
    }
    return s;
}
```