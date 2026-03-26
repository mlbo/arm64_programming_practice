# Lab 03-5: memcpy 实现

## 实验目标

使用 LDR/STR 后变基模式实现内存拷贝。

## 编译与运行

```bash
make
make run
```

## 核心内容

### memcpy 实现

```asm
my_memcpy_test:
    mov    x1, 0x80000       // 源地址
    mov    x2, 0x200000      // 目标地址
    add    x3, x1, 32        // 结束地址

copy_loop:
    ldr    x4, [x1], #8      // 加载并移动源指针
    str    x4, [x2], #8      // 存储并移动目标指针
    cmp    x1, x3
    b.cc   copy_loop
    ret
```

## 要点

- 使用后变基自动移动指针
- 每次拷贝 8 字节
- 循环直到完成