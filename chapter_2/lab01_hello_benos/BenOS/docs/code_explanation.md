# 代码说明

## boot.S - 启动汇编

### 入口点 `_start`

```asm
_start:
    mrs    x0, mpidr_el1
    and    x0, x0, #0xFF
    cbz    x0, master
    b      proc_hang
```

**作用**:
1. 读取 `MPIDR_EL1` 获取核心 ID
2. 只有 CPU0 进入 `master`，其他核心挂起

### BSS 清零

```asm
master:
    adr    x0, bss_begin
    adr    x1, bss_end
    sub    x1, x1, x0
    bl     memzero
```

**作用**:
- 清零 BSS 段，确保未初始化的全局变量为零

### 栈指针设置

```asm
mov    sp, #LOW_MEMORY
```

**作用**:
- 设置栈指针到 2MB 位置
- `LOW_MEMORY = 2 * SECTION_SIZE = 2MB`

---

## mm.S - 内存操作

### memzero 函数

```asm
memzero:
    str    xzr, [x0], #8    // 存储 0 并后移 8 字节
    subs   x1, x1, #8       // 计数器减 8
    b.gt   memzero          // 循环直到完成
    ret
```

**作用**:
- 将指定内存区域清零
- 每次写入 8 字节 (64 位)
- 使用后变基寻址 `[x0], #8`

---

## pl_uart.cpp - UART 驱动

### UartDriver 类

```cpp
class UartDriver {
public:
    void init();
    void send(char c);
    auto recv() -> char;
    void send_string(const char* str);
    static auto instance() -> UartDriver&;
private:
    UartDriver() = default;
};
```

**设计要点**:
- 单例模式，全局只有一个实例
- 通过 `instance()` 获取引用

### init() - 初始化流程

1. 配置 GPIO14/15 为 Alt0 模式
2. 设置上拉/下拉电阻
3. 禁用 UART
4. 设置波特率 (IBRD=26, FBRD=3)
5. 配置数据格式 (8N1)
6. 启用 UART

### send() - 发送字符

```cpp
void UartDriver::send(char c) {
    while (io::readl(U_FR_REG) & (1 << 5))  // 等待发送 FIFO 有空间
        ;
    io::writel(c, U_DATA_REG);              // 写入数据
}
```

### recv() - 接收字符

```cpp
auto UartDriver::recv() -> char {
    while (io::readl(U_FR_REG) & (1 << 4))  // 等待接收 FIFO 有数据
        ;
    return io::readl(U_DATA_REG) & 0xFF;    // 读取数据
}
```

---

## kernel.cpp - 内核入口

```cpp
extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Welcome BenOS!\r\n");

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}
```

**说明**:
- `extern "C"` 确保 C 链接，可被汇编调用
- 初始化 UART 后输出欢迎信息
- 进入回显循环