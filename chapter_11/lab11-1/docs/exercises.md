# 练习题

## 基础题

### 1. 读取 CurrentEL

编写代码读取并打印当前异常级别。

### 2. EL 字符串

实现 `el_to_string` 函数，将 EL 值转换为可读字符串。

## 进阶题

### 3. 读取其他系统寄存器

读取并打印以下寄存器：
- SPSel
- DAIF
- NZCV

### 4. EL 特定寄存器

编写代码检测当前 EL 并读取 EL 特定的寄存器：
- EL1: SCTLR_EL1, TTBR0_EL1
- EL2: SCTLR_EL2, TTBR0_EL2
- EL3: SCTLR_EL3, TTBR0_EL3

## 挑战题

### 5. EL 切换

实现从 EL3 下降到 EL1 的代码。