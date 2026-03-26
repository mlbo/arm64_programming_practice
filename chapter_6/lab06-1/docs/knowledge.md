# 知识点：地址加载指令

## 1. ADR 指令

```asm
ADR Xd, label
```

- 计算 `PC + offset`，结果存入 Xd
- 范围：±1MB
- 不依赖内存访问

## 2. ADRP 指令

```asm
ADRP Xd, label
```

- 计算 `(PC & ~0xFFF) + (label & ~0xFFF)`
- 结果是 4KB 对齐的页基址
- 需要配合 ADD 使用获取完整地址

```asm
adrp x0, my_data        // 页基址
add x0, x0, #:lo12:my_data  // 加上低 12 位
```

## 3. LDR 伪指令

```asm
LDR Xd, =label
```

- 汇编器会在 literal pool 中存放地址
- 通过 PC 相对加载获取地址
- 适用于任意地址

## 4. 对比

| 指令 | 范围 | 特点 |
|------|------|------|
| ADR | ±1MB | 不访问内存 |
| ADRP + ADD | ±4GB | 两步，不访问内存 |
| LDR =label | 任意 | 需要访问 literal pool |