# 练习题

## 练习 1: 地址计算

假设 `my_data` 位于地址 0x800100，当前 PC 为 0x800080。

计算以下指令的结果：
1. `adr x0, my_data` 后 x0 的值
2. `adrp x1, my_data` 后 x1 的值
3. `add x1, x1, #:lo12:my_data` 后 x1 的值

## 练习 2: 范围限制

为什么 ADR 只能访问 ±1MB 范围内的地址？这与 ARM64 指令编码有什么关系？

## 练习 3: Literal Pool

LDR 伪指令会在 literal pool 中存放数据。分析编译后的代码，找出 literal pool 的位置。

## 练习 4: 远距离访问

如果一个数据位于 4MB 之外，使用哪种方法访问最合适？

## 练习 5: 位置无关代码

ADRP 指令在位置无关代码 (PIC) 中有什么优势？