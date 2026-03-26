# Lab 04-1: Label 访问

## 实验目标

学习汇编中访问 Label 和数据的多种方式。

## 编译与运行

```bash
make
make run
```

## 核心内容

### 1. 加载大数

```asm
ldr x1, =0xffff0000ffff0000
```

### 2. 位操作初始化

```asm
ldr x2, =(1<<0) | (1<<2) | (1<<20) | (1<<40) | (1<<55)
```

### 3. 访问宏定义

```asm
ldr x0, =my_label   // 加载宏值
ldr x1, my_label    // PC 相对加载
```

### 4. 访问字符串

```asm
ldr x0, string1     // 加载 ASCII
ldr x1, =string1    // 加载地址
```

### 5. 访问数据

```asm
ldr x0, my_data     // 加载值
ldr x1, =my_data    // 加载地址
```