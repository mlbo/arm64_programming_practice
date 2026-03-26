# 练习题

## 练习 1: 验证修复

1. 编译并运行 lab05-3-fixed
2. 确认程序正常运行，不会跑飞
3. 使用 GDB 观察 x30 的变化

## 练习 2: 实现栈保存版本

修改 bl_test，使用 `stp/ldp` 保存和恢复 x30：

```asm
bl_test:
    // TODO: 使用栈保存 x30
    mov x0, 1
    mov x1, 3
    bl csel_test
    // TODO: 从栈恢复 x30
    ret
```

## 练习 3: 多层调用

分析以下代码，每个函数应该如何保存 x30？

```asm
func_a:
    bl func_b
    bl func_c
    ret

func_b:
    // 不调用其他函数
    add x0, x0, #1
    ret

func_c:
    bl func_d
    ret

func_d:
    // 不调用其他函数
    sub x0, x0, #1
    ret
```

## 练习 4: 内联汇编

使用 C++ 内联汇编实现一个安全的调用包装器：

```cpp
template<typename Func, typename... Args>
auto safe_call(Func func, Args... args) -> decltype(func(args...)) {
    // 使用内联汇编保护 x30
}
```

## 练习 5: 性能对比

比较两种保存方式的性能差异：
1. 临时寄存器方式
2. 栈保存方式

哪种更快？为什么？