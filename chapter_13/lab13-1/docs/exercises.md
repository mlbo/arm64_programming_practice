# GICv2 中断控制器 - 练习题

## 练习 1: 中断类型识别

**问题：** 根据中断 ID，判断中断类型 (SGI/PPI/SPI):

```cpp
const char* get_irq_type_string(u32 irq_id) {
    // TODO: 实现
}
```

**测试用例:**
- `irq_id = 5` → "SGI"
- `irq_id = 27` → "PPI"
- `irq_id = 65` → "SPI"

---

## 练习 2: 中断优先级计算

**问题：** GICv2 的中断优先级是 8 位值 (0-255)。解释以下代码的含义：

```cpp
writel(0xA0A0A0A0, GICD_BASE + GICD_IPRIORITYR + offset);
```

**提示：** 一个寄存器控制 4 个中断的优先级，每个中断 8 位。

---

## 练习 3: 中断路由设置

**问题：** 实现函数 `set_irq_target(u32 irq_id, u8 cpu_mask)`，设置中断路由到指定 CPU。

**要求：**
1. 使用 GICD_ITARGETSR 寄存器
2. 支持多 CPU 路由 (cpu_mask 可以是多个 CPU 的组合)

---

## 练习 4: Spurious Interrupt 处理

**问题：** 当读取 GICC_IAR 返回 1023 时表示什么？应该如何处理？

```cpp
void handle_irq() {
    u32 irq_id = gic.acknowledge();
    if (irq_id == 1023) {
        // TODO: 如何处理?
    }
}
```

---

## 练习 5: 中断嵌套

**问题：** 分析当前实现是否支持中断嵌套？如果需要支持，需要修改哪些地方？

**提示：** 考虑：
1. 在中断处理过程中是否屏蔽了其他中断？
2. 如何保存和恢复中断状态？

---

## 练习 6: 多核中断处理

**问题：** 在多核系统中，如何确保某个 SPI 中断只被一个 CPU 处理？

**提示：** 考虑 GICD_ITARGETSR 的设置方式。

---

## 练习 7: SGI 使用

**问题：** 实现 `send_sgi(u32 sgi_id, u32 target_cpu)` 函数，发送软件中断到指定 CPU。

**提示：** 使用 GICD_SGIR 寄存器。

```cpp
void send_sgi(u32 sgi_id, u32 target_cpu) {
    u32 val = (target_cpu << 16) | sgi_id;
    writel(val, GICD_BASE + GICD_SGIR);
}
```

---

## 练习 8: 中断统计

**任务：** 实现 GIC 中断统计功能，记录每个中断的发生次数。

```cpp
class GicV2 {
    // ...
private:
    u32 irq_stats_[1024] = {0};  // 中断统计

public:
    void print_stats() {
        for (u32 i = 0; i < irq_count_; i++) {
            if (irq_stats_[i] > 0) {
                printk("IRQ %u: %u times\n", i, irq_stats_[i]);
            }
        }
    }
};
```

---

## 练习 9: 中断亲和性调试

**问题：** 编写函数 `dump_irq_affinity(u32 irq_id)`，打印指定中断的路由配置。

```cpp
void dump_irq_affinity(u32 irq_id) {
    u32 offset = GICD_ITARGETSR + (irq_id / 4) * 4;
    u32 val = readl(GICD_BASE + offset);
    u8 target = (val >> ((irq_id % 4) * 8)) & 0xFF;
    printk("IRQ %u target: CPU mask 0x%02x\n", irq_id, target);
}
```

---

## 练习 10: 错误处理

**问题：** 在 `enable_irq()` 和 `disable_irq()` 中添加参数检查：

```cpp
auto GicV2::enable_irq(u32 irq_id) -> int {
    // TODO: 检查 irq_id 是否有效
    // 返回负数表示错误
}
```