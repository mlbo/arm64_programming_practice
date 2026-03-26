# 代码说明

## 1. ldr_test 函数

演示 LDR 指令的各种寻址模式。

```asm
// 基础寻址
ldr x0, [x1]           // 直接寻址
ldr x2, [x1, #8]       // 立即数偏移
ldr x4, [x1, x3]       // 寄存器偏移
ldr x5, [x1, x3, lsl #3]  // 带移位的寄存器偏移

// 前变基
ldr x6, [x1, #8]!      // x1 先加 8，再读取

// 后变基
ldr x7, [x1], #8       // 先读取，x1 再加 8
```

## 2. my_memcpy_test 函数

演示使用后变基寻址进行内存拷贝。

```asm
1:
    ldr x4, [x1], #8    // 读取源地址，x1 += 8
    str x4, [x2], #8    // 写入目标地址，x2 += 8
    cmp x1, x3
    b.cc 1b             // 如果 x1 < x3，继续循环
```

## 3. access_label_test 函数

演示 Label 的各种访问方式。

```asm
// 加载立即数
ldr x1, =0xffff0000ffff0000    // 使用 = 前缀加载大立即数

// 加载宏定义
ldr x0, =my_label    // 加载宏的值
ldr x1, my_label     // 加载 PC + 偏移地址的值

// 加载字符串地址
ldr x1, =string1     // 加载字符串地址
```

## 4. add_inst_test 函数

演示 ADDS 和 ADC 指令的条件标志位。

```asm
adds x0, x1, x1      // x0 = x1 + x1，设置标志位
adc x3, xzr, xzr     // x3 = 0 + 0 + C (进位标志)
```

## 5. compare_and_return 函数

演示 CMP 和 SBC 指令的综合运用。

```asm
cmp x0, x1           // 比较 x0 和 x1
sbc x0, xzr, xzr     // 根据 C 标志设置返回值
```

- 当 x0 >= x1 时，C=1，返回 0
- 当 x0 < x1 时，C=0，返回 0xffffffffffffffff