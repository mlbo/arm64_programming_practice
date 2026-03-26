# 定时器中断 - 知识点讲解

## 1. ARM Generic Timer 架构

ARM Generic Timer 是 ARM 架构的标准定时器，提供:
- 系统计数器 (System Counter)
- 每个核心一组定时器

```
+-----------------+
| System Counter  |  <-- 全局计数器，所有核心共享
+--------+--------+
         |
    +----+----+----+----+
    |    |    |    |
+---++--++---++---++---+
|EL0|EL1|EL2|EL3| ...
|Tim|Tim|Tim|Tim|
+---++--++---++---++---+
```

## 2. 定时器类型

| 定时器 | 寄存器前缀 | 说明 |
|-------|----------|------|
| EL1 Physical Timer | CNTP | 物理定时器 (EL1) |
| EL1 Virtual Timer | CNTV | 虚拟定时器 |
| EL2 Physical Timer | CNTHP | Hypervisor 定时器 |
| EL3 Physical Timer | CNTPS | 安全定时器 |

本实验使用 **EL1 Physical Timer**。

## 3. 关键寄存器

### CNTFRQ_EL0 - 频率寄存器

定时器频率 (通常 54MHz on RPi4):

```cpp
u64 freq;
asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
```

### CNTPCT_EL0 - 当前计数值

读取当前物理计数器值:

```cpp
u64 count;
asm volatile("mrs %0, cntpct_el0" : "=r"(count));
```

### CNTP_TVAL_EL0 - 定时器值

设置定时器触发时间 (相对值):

```cpp
// 设置 1 秒后触发
u64 freq = get_timer_frequency();
asm volatile("msr cntp_tval_el0, %0" :: "r"(freq));
```

### CNTP_CVAL_EL0 - 定时器比较值

设置定时器触发时间 (绝对值):

```cpp
u64 count, freq;
asm volatile("mrs %0, cntpct_el0" : "=r"(count));
asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
asm volatile("msr cntp_cval_el0, %0" :: "r"(count + freq));
```

### CNTP_CTL_EL0 - 定时器控制寄存器

```
Bit 0: ENABLE  - 定时器使能
Bit 1: IMASK   - 中断屏蔽 (1=屏蔽)
Bit 2: ISTATUS - 定时器状态 (1=已触发)
```

```cpp
// 使能定时器
asm volatile("msr cntp_ctl_el0, %0" :: "r"(1));
```

## 4. 定时器中断处理流程

```
1. 初始化
   ├─ 设置定时器频率
   ├─ 设置定时器值 (TVAL)
   └─ 使能定时器 (CTL.ENABLE=1)

2. 使能中断
   └─ GIC 使能 IRQ 30

3. 中断发生
   ├─ GIC 触发 IRQ
   ├─ CPU 跳转到异常向量
   ├─ kernel_entry 保存寄存器
   ├─ 读取 GICC_IAR 获取 IRQ ID
   ├─ 处理定时器中断
   ├─ 重置定时器值
   ├─ 写入 GICC_EOIR
   └─ kernel_exit 恢复寄存器
```

## 5. 定时器间隔计算

```cpp
// 设置 10ms 间隔
constexpr u64 HZ = 250;  // 250 ticks per second
constexpr u64 NSEC_PER_SEC = 1000000000ULL;
u64 interval = freq / HZ;  // 每次中断的计数周期
```

## 6. 树莓派本地定时器

除了 ARM Generic Timer，树莓派还有本地定时器:

```cpp
#define TIMER_CNTRL0  0xFF800040
#define TIMER_CLO     0xFF800004
#define TIMER_C1      0xFF800010

// 使能定时器中断
writel(CNT_PNS_IRQ, TIMER_CNTRL0);
```

## 7. 中断路由

树莓派 4 的中断路由:
- IRQ 30: ARM Generic Timer (PPI)
- IRQ 65: System Timer 1 (SPI)

需要在 GIC 中使能对应中断。

## 8. 定时器精度

ARM Generic Timer 的精度取决于:
- CNTFRQ_EL0 频率
- System Counter 实现

典型值:
- RPi4: 54 MHz (~18.5 ns 分辨率)
- QEMU: 62.5 MHz (16 ns 分辨率)