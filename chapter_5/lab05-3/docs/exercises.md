# 练习题

## 练习 1: 分析 Bug

运行 lab05-3，观察程序行为。使用 GDB 单步调试：

```bash
# 终端 1
make debug

# 终端 2
aarch64-linux-gnu-gdb benos.elf
(gdb) target remote :1234
(gdb) break bl_test
(gdb) continue
(gdb) stepi
```

问题：
1. 执行 `bl csel_test` 后，x30 的值是多少？
2. 程序最终跑飞到哪里？

## 练习 2: 修复 Bug

修改 `bl_test` 函数，使其正确返回。

提示：使用 `stp x29, x30, [sp, #-16]!` 保存返回地址。

## 练习 3: 多层嵌套

分析以下代码，找出潜在问题：

```asm
func_a:
    bl func_b
    ret

func_b:
    bl func_c
    ret

func_c:
    bl func_d
    ret

func_d:
    ret
```

## 练习 4: 叶子函数优化

什么是"叶子函数"？为什么叶子函数不需要保存 x30？

## 练习 5: 栈深度计算

如果一个函数调用了 3 个子函数，每个子函数都需要保存 x29 和 x30，栈需要预留多少空间？