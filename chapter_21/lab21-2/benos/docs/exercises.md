# BenOS 练习题

本文档提供一系列练习题，帮助读者深入理解BenOS和操作系统开发。

## 基础练习

### 练习1：修改启动信息

**目标**：修改内核启动时打印的欢迎信息。

**步骤**：
1. 打开 `init/main.cpp`
2. 找到 `g_uart.send_string()` 调用
3. 修改欢迎信息
4. 重新编译并运行

**思考**：
- 为什么使用 `g_uart.send_string()` 而不是 `g_console.printf()`？
- 在什么阶段可以使用printf格式的输出？

---

### 练习2：添加新的内核线程

**目标**：创建第二个内核线程，打印不同的信息。

**要求**：
1. 创建一个新的线程函数
2. 使用 `g_process_manager.create_process()` 创建线程
3. 观察两个线程的输出

**示例代码**：
```cpp
extern "C" void my_kernel_thread(void) {
    int count = 0;
    while (true) {
        delay(5000000);
        g_console.printf("MyThread: count=%d\n", count++);
    }
}
```

**思考**：
- 两个线程是否交替执行？为什么？
- 如何实现线程的时间片轮转调度？

---

### 练习3：扩展打印功能

**目标**：为 `Console::printf` 添加新的格式说明符。

**要求**：
1. 阅读 `lib/printk.cpp` 中的 `myprintf` 函数
2. 添加对 `%b`（二进制输出）的支持
3. 测试：`g_console.printf("Binary: %b\n", 10);` 应输出 `Binary: 1010`

**提示**：
- 参考 `%x`（十六进制）的实现
- 二进制基数为2

---

### 练习4：内存统计

**目标**：添加命令显示内存使用情况。

**要求**：
1. 使用 `g_page_allocator.total_pages()` 和 `g_page_allocator.free_pages()`
2. 在内核启动后打印内存统计信息
3. 计算并显示已使用内存的百分比

**示例输出**：
```
Memory Statistics:
  Total pages: 8192
  Free pages: 8190
  Used: 0.02%
```

---

## 进阶练习

### 练习5：实现简单调度器

**目标**：实现简单的时间片轮转调度器。

**要求**：
1. 在定时器中断处理函数中调用调度器
2. 实现简单的轮转算法
3. 让多个进程交替执行

**提示**：
```c
void schedule(void) {
    int i;
    struct task_struct *next = NULL;

    // 找到下一个可运行的进程
    for (i = current->pid + 1; i < NR_TASK; i++) {
        if (g_task[i] && g_task[i]->state == TASK_RUNNING) {
            next = g_task[i];
            break;
        }
    }

    // 循环查找
    if (!next) {
        for (i = 0; i <= current->pid; i++) {
            if (g_task[i] && g_task[i]->state == TASK_RUNNING) {
                next = g_task[i];
                break;
            }
        }
    }

    if (next && next != current) {
        switch_to(next);
    }
}
```

**思考**：
- 如何确定时间片的大小？
- 如何避免调度过于频繁？

---

### 练习6：进程睡眠

**目标**：实现 `sleep()` 函数，让进程睡眠指定的时间。

**要求**：
1. 添加进程状态 `TASK_INTERRUPTIBLE`
2. 实现睡眠队列
3. 在定时器中断中唤醒睡眠的进程

**提示**：
```c
void sleep(unsigned int seconds) {
    current->state = TASK_INTERRUPTIBLE;
    current->wake_time = get_current_time() + seconds;
    schedule();  // 切换到其他进程
}
```

---

### 练习7：进程退出

**目标**：实现进程退出功能。

**要求**：
1. 实现 `do_exit()` 函数
2. 将进程状态设置为 `TASK_ZOMBIE`
3. 释放进程资源

**示例代码**：
```c
void do_exit(void) {
    current->state = TASK_ZOMBIE;
    schedule();  // 永不返回
}
```

**思考**：
- 僵尸进程的资源何时释放？
- 父进程如何获知子进程的退出状态？

---

### 练习8：简单命令行

**目标**：实现简单的命令行界面。

**要求**：
1. 接收用户输入
2. 解析命令
3. 执行相应操作

**示例命令**：
- `help` - 显示帮助
- `mem` - 显示内存信息
- `ps` - 显示进程列表
- `clear` - 清屏

**提示**：
```cpp
void process_command(const char* cmd) {
    if (strcmp(cmd, "help") == 0) {
        g_console.printf("Commands: help, mem, ps, clear\n");
    } else if (strcmp(cmd, "mem") == 0) {
        g_console.printf("Free pages: %u\n", g_page_allocator.free_pages());
    }
    // ...
}
```

---

## 挑战练习

### 练习9：虚拟内存

**目标**：实现基本的虚拟内存管理。

**要求**：
1. 启用MMU
2. 创建页表
3. 实现虚拟地址到物理地址的映射

**提示**：
- 阅读ARM Architecture Reference Manual中的内存管理单元章节
- 参考Linux内核的页表管理代码

---

### 练习10：系统调用

**目标**：实现系统调用机制。

**要求**：
1. 定义系统调用号
2. 实现系统调用处理函数
3. 用户程序通过SVC指令调用系统调用

**示例**：
```c
// 用户态调用
int write(int fd, const char* buf, int count) {
    return syscall(SYS_write, fd, buf, count);
}

// 内核态处理
long sys_write(int fd, const char* buf, int count) {
    // 实现写入逻辑
}
```

---

### 练习11：进程间通信

**目标**：实现进程间通信机制。

**要求**：
1. 实现简单的信号量
2. 实现生产者-消费者模型
3. 使用共享内存传递数据

**提示**：
```c
struct semaphore {
    int count;
    struct task_struct *wait_list;
};

void down(struct semaphore *sem) {
    if (sem->count > 0) {
        sem->count--;
    } else {
        // 加入等待队列
        current->state = TASK_INTERRUPTIBLE;
        schedule();
    }
}

void up(struct semaphore *sem) {
    sem->count++;
    // 唤醒等待的进程
}
```

---

### 练习12：文件系统

**目标**：实现简单的文件系统。

**要求**：
1. 定义文件系统数据结构
2. 实现文件的创建、读写、删除
3. 支持目录结构

**提示**：
- 可以实现简单的RAM文件系统
- 参考Linux的VFS（虚拟文件系统）设计

---

## 思考题

### 理论问题

1. **异常级别**
   - 为什么操作系统需要运行在EL1而不是EL0？
   - EL2和EL3在实际系统中有什么用途？

2. **进程切换**
   - 为什么需要保存x19-x28寄存器？这些寄存器有什么特殊之处？
   - 如果忘记保存某个寄存器会发生什么？

3. **内存管理**
   - 为什么页分配器使用引用计数而不是简单的空闲标志？
   - 如何检测内存泄漏？

4. **中断处理**
   - 为什么中断处理要尽可能快？
   - 什么是中断上下文？与进程上下文有什么区别？

### 设计问题

1. **进程调度**
   - 如何设计优先级调度？
   - 如何防止进程饿死？

2. **内存保护**
   - 如何防止一个进程访问另一个进程的内存？
   - 如何实现用户空间和内核空间的隔离？

3. **可移植性**
   - BenOS的哪些部分是平台相关的？
   - 如何设计可移植的驱动接口？

---

## 调试练习

### 练习D1：调试启动过程

**目标**：使用GDB跟踪启动过程。

**步骤**：
1. 启动QEMU调试模式
2. 在 `_start` 设置断点
3. 单步执行，观察EL切换过程
4. 记录每一步的寄存器变化

**问题**：
- `eret` 指令后，PC和SPSR如何变化？
- `CurrentEL` 寄存器的值如何变化？

---

### 练习D2：调试进程切换

**目标**：跟踪进程切换过程。

**步骤**：
1. 在 `cpu_switch_to` 设置断点
2. 观察 `prev` 和 `next` 的 `task_struct` 地址
3. 单步执行汇编代码
4. 确认寄存器正确保存和恢复

---

### 练习D3：调试定时器中断

**目标**：跟踪定时器中断处理过程。

**步骤**：
1. 在定时器中断处理函数设置断点
2. 观察中断发生时的寄存器状态
3. 确认中断处理完成后正确返回

---

## 参考资料

- 《ARM64体系结构：编程与实践》
- ARM Architecture Reference Manual ARMv8
- Linux内核源码 (特别是arch/arm64目录)
- 《Operating Systems: Three Easy Pieces》
- 《Understanding the Linux Kernel》