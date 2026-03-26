# Lab 23-4: SVE strcmp 实现

## 实验目标

- 使用 SVE 实现高效的字符串比较
- 理解 First Fault Register (FFR) 的作用
- 掌握 SVE 条件比较指令

## 实验内容

本实验演示如何使用 SVE 实现 strcmp 函数，利用 FFR 处理字符串结束检测。

### 关键技术点

1. **FFR**: First Fault Register，检测首次访存失败
2. **条件比较**: `cmpeq`, `cmpne`
3. **Predicate 逻辑**: `orrs`, `brkb`

## 构建与运行

```bash
make && ./sve_strcmp_test
```

## 预期输出

```
String 1: "hello"
String 2: "hello"
C strcmp: 0
SVE strcmp: 0
Results match!

String 1: "hello"
String 2: "world"
C strcmp: -15
SVE strcmp: -15
Results match!
```