# Lab10-4: LSE 原子指令 (ARMv8.1-A)

## 实验目的

学习 ARMv8.1-A 引入的 LSE (Large System Extensions) 原子指令。

## 实验内容

- STSET, LDSET 指令
- CAS 指令
- 与 LL/SC 的性能对比

## 知识点

### LSE 指令优势

ARMv8.1-A 引入了硬件原子指令，相比 LL/SC：
- 更好的性能（单条指令）
- 避免 ABA 问题
- 更低的功耗

### STADD 指令

```asm
stadd w0, [x1]  ; 原子加法，相当于 atomic_fetch_add
```

## 运行方法

```bash
make && make run
```