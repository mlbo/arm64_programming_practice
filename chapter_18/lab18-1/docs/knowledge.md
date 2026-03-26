# 内存模型知识点

## 1. 内存一致性模型

### 强内存模型 vs 弱内存模型

| 模型类型 | 特点 | 代表架构 |
|----------|------|----------|
| 强内存模型 | 程序顺序 = 执行顺序 | x86, SPARC |
| 弱内存模型 | 允许特定重排 | ARM, POWER, RISC-V |

### ARM64 允许的重排

```
允许的重排:
Load  -> Load   ✓
Load  -> Store  ✓
Store -> Store  ✓
Store -> Load   ✓

必须使用屏障来阻止重排
```

## 2. 内存屏障指令

### dmb (Data Memory Barrier)

```
dmb ish    : 完整屏障 (所有内存操作)
dmb ishst  : 存储屏障 (仅 Store)
dmb ishld  : 加载屏障 (仅 Load)
```

**作用**: 确保 dmb 之前的内存访问在 dmb 之后的内存访问之前完成

### dsb (Data Synchronization Barrier)

**作用**: 等待所有之前的内存访问完成

```
dsb ish    : 内部共享域
dsb sy     : 完整系统
```

### isb (Instruction Synchronization Barrier)

**作用**: 清除指令流水线，确保之后取指令看到之前的状态

## 3. Acquire/Release 语义

### Release 语义

确保之前的读写操作不会被重排到 Release 操作之后

```cpp
// Release store
stlr w0, [x1]  // 带有 Release 语义的存储
```

### Acquire 语义

确保之后的读写操作不会被重排到 Acquire 操作之前

```cpp
// Acquire load
ldar w0, [x1]  // 带有 Acquire 语义的加载
```

## 4. Litmus 测试

### Message Passing (MP)

```
P0: data = 1; flag = 1;
P1: r1 = flag; r2 = data;

允许结果: r1=1, r2=0 (观察到重排)
```

### Store Buffer (SB)

```
P0: x = 1; r1 = y;
P1: y = 1; r2 = x;

允许结果: r1=0, r2=0 (观察到重排)
```

### Load Buffer (LB)

```
P0: r1 = y; x = 1;
P1: r2 = x; y = 1;

允许结果: r1=1, r2=1 (观察到重排)
```

## 5. herdtools7

### 安装

```bash
sudo apt install opam
opam init
opam install herdtools7
eval $(opam config env)
```

### 使用

```bash
# 基本用法
herd7 -cat aarch64.cat test.litmus

# 指定架构
herd7 -arch aarch64 test.litmus
```

### Litmus 文件格式

```
AArch64
"Test name"
{
  x = 0; y = 0;  // 初始状态
}
P0 | P1 ;
MOV W0, #1 | LDR W0, [X1] ;
STR W0, [X1] | LDR W1, [X2] ;
exists (1:W0=1 /\ 1:W1=0)  // 检查条件
```

## 6. Linux 内核内存屏障

### smp_wmb() - 写屏障

```c
data = 1;
smp_wmb();  // 确保之前写完成
flag = 1;
```

### smp_rmb() - 读屏障

```c
r1 = flag;
smp_rmb();  // 确保之后读不重排
r2 = data;
```

### smp_mb() - 完整屏障

```c
smp_mb();  // 确保所有内存操作有序
```