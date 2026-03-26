# 异常级别知识点

## 1. 异常级别层次

```
          +------+
          | EL3  |  Secure Monitor
          +------+
            |
          +------+
          | EL2  |  Hypervisor
          +------+
            |
          +------+
          | EL1  |  Kernel (OS)
          +------+
            |
          +------+
          | EL0  |  Applications
          +------+
```

## 2. CurrentEL 寄存器

### 格式

```
+-------+-------+-------+-------+
| ...   | ...   | EL    | 00    |
+-------+-------+-------+-------+
  63-4    3      2-1     0
```

### 读取方法

```asm
mrs x0, CurrentEL
lsr x0, x0, #2   ; 右移2位得到 EL 值
```

### C++ 实现

```cpp
auto get_current_el() -> u64 {
    u64 el;
    asm volatile("mrs %0, CurrentEL" : "=r" (el));
    return (el >> 2) & 0x3;
}
```

## 3. EL 切换

### EL3 -> EL2

```asm
// 设置 SCR_EL3
mov x0, #(1 << 10)  // RW=1, 下级使用 AArch64
msr scr_el3, x0

// 设置返回地址
adr x0, el2_entry
msr elr_el3, x0

// 设置 SPSR
mov x0, #0b000010101  // EL2, spsel=1
msr spsr_el3, x0

eret
```

### EL2 -> EL1

```asm
// 设置返回地址
adr x0, el1_entry
msr elr_el2, x0

// 设置 SPSR
mov x0, #0b000001001  // EL1, spsel=1
msr spsr_el2, x0

eret
```

## 4. 栈指针选择

每个 EL 都有两个栈指针：
- SP_EL0: EL0 栈指针
- SP_ELx: 当前 EL 的栈指针

使用 `SPSel` 寄存器选择：

```asm
msr spsel, #1   ; 使用 SP_ELx
msr spsel, #0   ; 使用 SP_EL0
```