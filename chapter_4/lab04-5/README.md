# Lab 04-5: BFI/UBFX/SBFX 位段指令

## 实验目标

学习 ARM64 位段操作指令。

## 编译与运行

```bash
make
make run
```

## 核心内容

### BFI (Bit Field Insert)

```asm
bfi x0, x1, #8, #4    // 将 x1[3:0] 插入 x0[11:8]
```

### UBFX (Unsigned Bit Field Extract)

```asm
ubfx x3, x2, #4, #8   // 提取 x2[11:4]，零扩展
```

### SBFX (Signed Bit Field Extract)

```asm
sbfx x4, x2, #4, #8   // 提取 x2[11:4]，符号扩展
```

## 应用

- 寄存器字段解析
- 数据打包/解包
- CPU 特性检测