# Lab 05-3: BL 指令与返回地址 (有 Bug)

## 实验目的

1. 理解 BL 指令对 x30 (LR) 的影响
2. 认识嵌套调用时返回地址丢失的问题
3. 学习调试技巧

## Bug 说明

```asm
.global bl_test
bl_test:
    mov x0, 1
    mov x1, 3
    bl csel_test      // BL 会修改 x30
    ret               // 返回地址已丢失！程序会跑飞
```

**问题**: `bl csel_test` 会将返回地址写入 x30，覆盖了 `bl_test` 的返回地址。

## 预期行为

程序会在 `bl_test` 的 `ret` 指令处跑飞，因为 x30 已被覆盖。

## 构建与运行

```bash
make && make run
```

## 修复方案

参见 lab05-3-fixed 的正确实现。