# 练习题

## 基础题

### 1. 启动流程

**问题**: 为什么启动地址是 `0x80000`？能否修改为其他地址？

**提示**: 考虑树莓派的启动流程和 GPU 的行为。

### 2. 多核处理

**问题**: 如果想让所有核心都执行 `kernel_main`，应该怎么修改？

```asm
// 当前代码
_start:
    mrs    x0, mpidr_el1
    and    x0, x0, #0xFF
    cbz    x0, master
    b      proc_hang
```

### 3. BSS 清零

**问题**: 为什么必须清零 BSS 段？如果不清零会发生什么？

### 4. 波特率

**问题**: 如果想把波特率改为 9600，IBRD 和 FBRD 应该设置为多少？

**计算公式**:
```
波特率分频 = UARTCLK / (16 * baud_rate)
整数部分 = floor(分频值)
小数部分 = round((分频值 - 整数部分) * 64)
```

## 进阶题

### 5. 栈指针

**问题**: `LOW_MEMORY = 2MB`，这个值是怎么确定的？能不能设置得更小？

### 6. UART FIFO

**问题**: 查看 PL011 手册，UART 的 FIFO 深度是多少？如何利用 FIFO 提高传输效率？

### 7. GPIO 模式

**问题**: GPIO14 可以配置为哪些功能？查看数据手册列出所有 Alt 模式。

## 实践题

### 8. 添加输出函数

**任务**: 在 `UartDriver` 类中添加 `send_hex(u32 value)` 方法，以十六进制格式输出数值。

```cpp
// 期望输出: send_hex(0x1234ABCD) 输出 "1234ABCD"
void send_hex(u32 value);
```

### 9. 支持 Pi 3B

**任务**: 当前代码已经通过 `CONFIG_BOARD_PI3B` 宏支持 Pi 3B。验证在 QEMU 中使用 Pi 3B 模式运行:

```bash
make clean
make board=rpi3
qemu-system-aarch64 -machine raspi3 -nographic -kernel benos.bin
```

### 10. 打印核心 ID

**任务**: 修改代码，让每个核心启动时打印自己的 ID。

**期望输出**:
```
CPU 0 started
CPU 1 started
CPU 2 started
CPU 3 started
Welcome BenOS!
```

**提示**: 需要考虑 UART 访问的竞争条件。

---

## 参考答案

### 练习 8 参考实现

```cpp
void UartDriver::send_hex(u32 value) {
    const char hex[] = "0123456789ABCDEF";
    char buf[9];

    for (int i = 7; i >= 0; --i) {
        buf[i] = hex[value & 0xF];
        value >>= 4;
    }
    buf[8] = '\0';

    send_string(buf);
}
```