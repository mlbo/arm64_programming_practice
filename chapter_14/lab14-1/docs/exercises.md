# 定时器中断 - 练习题

## 练习 1: 定时器频率

**问题：** 编写函数获取定时器频率:

```cpp
u64 get_timer_frequency() {
    u64 freq;
    // TODO: 使用 mrs 指令读取 cntfrq_el0
    return freq;
}
```

---

## 练习 2: 高精度延时

**任务：** 实现微秒级延时函数:

```cpp
void udelay(u64 us) {
    // TODO: 使用 cntpct_el0 实现精确延时
}
```

**测试:** 比较 `udelay(1000000)` 是否约为 1 秒。

---

## 练习 3: 定时器间隔计算

**问题：** 假设定时器频率为 54MHz，计算:
1. 1ms 对应多少 ticks?
2. 10ms 对应多少 ticks?
3. 1s 对应多少 ticks?

---

## 练习 4: 绝对时间定时器

**任务：** 使用 CNTP_CVAL_EL0 实现绝对时间定时:

```cpp
void set_timer_absolute(u64 target_count) {
    // TODO: 设置 CVAL 寄存器
}
```

**问题:** TVAL 和 CVAL 有什么区别？

---

## 练习 5: 多定时器

**问题：** 如何实现多个软件定时器？设计一个结构体:

```cpp
struct SoftTimer {
    u64 expire_count;    // 到期时间
    void (*callback)();  // 回调函数
    bool active;         // 是否激活
};

class SoftTimerManager {
    static constexpr u32 MAX_TIMERS = 16;
    SoftTimer timers_[MAX_TIMERS];

public:
    void add_timer(u64 delay_us, void (*callback)());
    void tick();  // 在定时器中断中调用
};
```

---

## 练习 6: 中断统计

**任务：** 实现中断统计功能:

```cpp
class IrqManager {
    u32 irq_count_[MAX_IRQ] = {0};

public:
    void dump_stats() {
        for (u32 i = 0; i < MAX_IRQ; i++) {
            if (irq_count_[i] > 0) {
                printk("IRQ %u: %u times\n", i, irq_count_[i]);
            }
        }
    }
};
```

---

## 练习 7: 定时器精度测试

**任务：** 测量定时器中断的实际间隔:

```cpp
void handle_timer_irq() {
    static u64 last_count = 0;
    u64 now = TimerDriver::get_count();

    if (last_count > 0) {
        u64 elapsed = now - last_count;
        printk("Elapsed: %lu ticks\n", elapsed);
    }
    last_count = now;

    reset_timer();
}
```

---

## 练习 8: 系统时钟

**任务：** 实现一个简单的系统时钟:

```cpp
class SystemClock {
    u64 boot_time_ns_{0};
    u64 ticks_{0};

public:
    void init();      // 读取启动时间
    void tick();      // 定时器中断时调用
    u64 uptime_ms();  // 返回启动后的毫秒数
};
```

---

## 练习 9: 中断嵌套

**问题：** 分析当前实现是否支持中断嵌套。如果需要支持，应该:
1. 在中断处理中重新使能中断
2. 使用优先级控制

```cpp
void handle_irq_nested() {
    // 保存当前中断状态
    // 使能更高优先级中断
    // ...
    // 恢复中断状态
}
```

---

## 练习 10: 定时器调试

**问题：** 如果定时器中断不工作，列出可能的检查点:

1. [ ] GIC 是否正确初始化?
2. [ ] IRQ 30 是否使能?
3. [ ] 定时器是否使能 (CNTP_CTL_EL0)?
4. [ ] ?