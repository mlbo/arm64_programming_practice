# 知识点: BFI/UBFX/SBFX

## BFI (Bit Field Insert)

```asm
bfi xd, xn, #lsb, #width
```

将 `xn[width-1:0]` 插入 `xd[lsb+width-1:lsb]`

## UBFX (Unsigned Bit Field Extract)

```asm
ubfx xd, xn, #lsb, #width
```

提取 `xn[lsb+width-1:lsb]`，高位清零

## SBFX (Signed Bit Field Extract)

```asm
sbfx xd, xn, #lsb, #width
```

提取 `xn[lsb+width-1:lsb]`，高位符号扩展

## 示例

```asm
// 数据: 0x5678abcd
// 提取 bits [11:4]

ubfx x3, x2, #4, #8   // 结果: 0x000000bc
sbfx x4, x2, #4, #8   // 结果: 0xffffffffffffffbc
```

## CPU 特性检测

```asm
mrs   x1, ID_AA64ISAR0_EL1
ubfx  x0, x1, #20, #4   // Atomic 扩展
ubfx  x2, x1, #4, #4    // AES 扩展
```

## C++ 等价实现

```cpp
template<typename T>
constexpr auto extract(T val, u32 lsb, u32 width) -> T {
    return (val >> lsb) & ((T(1) << width) - 1);
}
```