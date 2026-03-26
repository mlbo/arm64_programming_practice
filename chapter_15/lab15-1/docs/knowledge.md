# 缓存管理 - 知识点讲解

## 1. 缓存层次结构

ARM64 处理器通常有多级缓存:

```
CPU Core 0
    │
    ├─ L1 I-Cache (指令缓存)
    │   └─ ~32-64KB, 4-way
    │
    ├─ L1 D-Cache (数据缓存)
    │   └─ ~32-64KB, 4-way
    │
    └─ L2 Cache (统一缓存)
        └─ ~256KB-2MB, 8-16 way

CPU Core 1 ...
    └─ (类似结构)
```

## 2. 关键寄存器

### CTR_EL0 - 缓存类型寄存器

```
Bits [31:28]  CWG    - Cache Write-back Granule
Bits [27:24]  ERG    - Exclusives Reservation Granule
Bits [23:20]  DminLine - D-Cache 最小缓存行 (log2)
Bits [19:16]  L1Ip   - L1 指令缓存策略
Bits [3:0]    IminLine - I-Cache 最小缓存行 (log2)
```

```cpp
u64 ctr;
asm volatile("mrs %0, ctr_el0" : "=r"(ctr));

u32 line_size_d = 4 << ((ctr >> 16) & 0xF);
u32 line_size_i = 4 << (ctr & 0xF);
```

### CLIDR_EL1 - 缓存层次 ID 寄存器

```
Bits [31:27]  LoUU   - Level of Unification Uniprocessor
Bits [26:24]  LoC    - Level of Coherency
Bits [23:21]  LoUIS  - Level of Unification Inner Shareable
Bits [2:0]    Ctype1 - Level 1 缓存类型
Bits [5:3]    Ctype2 - Level 2 缓存类型
...
```

缓存类型 (Ctype):
- 0: No cache
- 1: Instruction cache only
- 2: Data cache only
- 3: Separate instruction and data caches
- 4: Unified cache

```cpp
u64 clidr;
asm volatile("mrs %0, clidr_el1" : "=r"(clidr));

u32 ctype1 = (clidr >> 0) & 0x7;
u32 ctype2 = (clidr >> 3) & 0x7;
```

### CCSIDR_EL1 - 缓存大小 ID 寄存器

需要先写入 CSSELR_EL1 选择缓存级别和类型。

```
Bits [31:13]  NumSets - 组数
Bits [12:3]   Associativity - 路数
Bits [2:0]    LineSize - 缓存行大小 (log2 - 4)
```

```cpp
// 选择 L1 数据缓存
u32 csselr = (0 << 1) | 0;  // Level 0 (L1), Data
asm volatile("msr csselr_el1, %0" :: "r"(csselr));
isb();

u64 ccsidr;
asm volatile("mrs %0, ccsidr_el1" : "=r"(ccsidr));

u32 sets = ((ccsidr >> 13) & 0x7FFF) + 1;
u32 ways = ((ccsidr >> 3) & 0x3FF) + 1;
u32 line_size = 1 << ((ccsidr & 0x7) + 4);
```

## 3. 缓存维护操作

### dc 指令

```asm
dc ivac, xt   // Invalidate by VA to PoC
dc civac, xt  // Clean and Invalidate by VA to PoC
dc cvac, xt   // Clean by VA to PoC
dc cvau, xt   // Clean by VA to PoU
dc isw, xt    // Invalidate by Set/Way
dc cisw, xt   // Clean and Invalidate by Set/Way
dc csw, xt    // Clean by Set/Way
```

### ic 指令

```asm
ic ivau, xt   // Invalidate by VA to PoU
ic iallu      // Invalidate All to PoU
ic ialluis    // Invalidate All to PoU Inner Shareable
```

## 4. 缓存操作使用场景

| 场景 | 操作 |
|-----|------|
| 修改代码后 | `ic ivau` + `dsb ish` |
| DMA 读前 | `dc cvac` |
| DMA 写后 | `dc ivac` |
| 进程切换 | `dc civac` (如果需要) |

## 5. 缓存行大小

缓存行大小决定了:
- 缓存操作的最小单位
- `dc` 指令操作的范围

典型值: 64 字节 ( Cortex-A72)

```cpp
u32 cache_line_size() {
    u64 ctr;
    asm volatile("mrs %0, ctr_el0" : "=r"(ctr));
    return 4 << ((ctr >> 16) & 0xF);
}
```

## 6. 缓存策略

### 指令缓存策略

| 策略 | 说明 |
|-----|------|
| VIPT | Virtual Index Physical Tag |
| PIPT | Physical Index Physical Tag |
| VPIPT | Virtual PIPT (ASID-tagged) |

```cpp
u32 get_icache_policy() {
    u64 ctr;
    asm volatile("mrs %0, ctr_el0" : "=r"(ctr));
    return (ctr >> 14) & 0x3;
}
```

## 7. PoC 与 PoU

- **PoC (Point of Coherency)**: 所有观察者都能看到一致数据的位置
- **PoU (Point of Unification)**: 指令和数据缓存统一的位置

```
    ┌─────────┐
    │  CPU    │
    │ L1 I$   │──┐
    │ L1 D$   │  │
    └─────────┘  │
         │       │
         ▼       │
    ┌─────────┐  │ PoU (L2 Unified)
    │   L2    │◄─┘
    └─────────┘
         │
         ▼
    ┌─────────┐
    │ Memory  │  PoC
    └─────────┘
```

## 8. 缓存刷新代码示例

### 按 VA 刷新

```cpp
void clean_and_invalidate_by_va(u64 va) {
    asm volatile("dc civac, %0" :: "r"(va));
    asm volatile("dsb ish");
    asm volatile("isb");
}
```

### 按组路刷新整个缓存

```cpp
void clean_all_dcache(u32 level) {
    // 选择缓存级别
    u32 csselr = (level << 1);
    asm volatile("msr csselr_el1, %0" :: "r"(csselr));
    isb();

    // 读取 CCSIDR
    u64 ccsidr;
    asm volatile("mrs %0, ccsidr_el1" : "=r"(ccsidr));

    u32 sets = ((ccsidr >> 13) & 0x7FFF) + 1;
    u32 ways = ((ccsidr >> 3) & 0x3FF) + 1;

    // 遍历所有 set/way
    for (u32 set = 0; set < sets; set++) {
        for (u32 way = 0; way < ways; way++) {
            u64 sw = (way << 32) | (set << 6) | (level << 1);
            asm volatile("dc cisw, %0" :: "r"(sw));
        }
    }

    asm volatile("dsb ish");
}
```