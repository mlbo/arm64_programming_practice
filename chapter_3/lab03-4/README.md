# Lab 03-4: LDR 加载 Label

## 实验目标

学习 LDR 指令加载 Label 的两种方式。

## 编译与运行

```bash
make
make run
```

## 核心内容

### 加载立即数

```asm
ldr x6, MY_LABEL     // PC 相对加载
```

从 PC + offset 地址加载值。

### 加载地址

```asm
ldr x7, =MY_LABEL    // 伪指令
```

将 `MY_LABEL` 的地址加载到寄存器。

## 区别

| 指令 | 结果 |
|------|------|
| `ldr x6, MY_LABEL` | 加载内存中的值 |
| `ldr x7, =MY_LABEL` | 加载地址值 |