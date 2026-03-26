# 练习题

## 基础题

### 1. perf c2c 安装

**问题**: 在你的系统上安装 perf 工具，并验证 perf c2c 子命令可用。

**步骤**:
```bash
# Ubuntu/Debian
sudo apt install linux-tools-common linux-tools-generic

# 验证
perf c2c record --help
```

---

### 2. 运行 perf c2c

**问题**: 对本实验程序运行 perf c2c，记录关键指标。

**命令**:
```bash
sudo perf c2c record ./perf_demo
sudo perf c2c report --stats
```

**记录以下数据**:
- Total Shared Cache Lines
- Load HITs on shared lines
- Store HITs on shared lines

---

### 3. 报告解读

**问题**: 解释 perf c2c report 输出中以下列的含义：
- Rmt HITM
- Lcl HITM
- Store

---

## 进阶题

### 4. 多种场景对比

**问题**: 修改程序，测试以下场景并对比 perf c2c 结果：

| 场景 | 线程数 | 迭代次数 | 结果 |
|------|--------|----------|------|
| A | 2 | 100,000,000 | ? |
| B | 4 | 100,000,000 | ? |
| C | 4 | 1,000,000,000 | ? |

---

### 5. 采样频率影响

**问题**: 使用不同采样频率运行 perf c2c：
```bash
perf c2c record -F 1000 ./perf_demo   # 低频
perf c2c record -F 10000 ./perf_demo  # 高频
```

对比结果准确性。

---

### 6. 缓存行大小验证

**问题**: 编写程序验证你系统的缓存行大小：
```bash
getconf LEVEL1_DCACHE_LINESIZE
cat /sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size
```

修改代码中的 `CACHE_LINE_SIZE` 测试影响。

---

## 挑战题

### 7. 定制 perf 事件

**问题**: 使用 `perf record` 监控特定硬件事件：
```bash
perf record -e cache-references,cache-misses ./perf_demo
perf report
```

分析缓存命中率。

---

### 8. 可视化分析

**问题**: 使用 perf 生成火焰图：
```bash
perf record -g ./perf_demo
perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg
```

---

### 9. 多 NUMA 节点分析

**问题**: 在 NUMA 系统上，使用 `numactl` 绑定 CPU：
```bash
numactl --cpunodebind=0 --membind=0 perf c2c record ./perf_demo
```

对比不同 NUMA 配置的性能差异。

---

## 实践题

### 10. 真实场景分析

**问题**: 找一个你常用的多线程程序，使用 perf c2c 分析是否存在 False Sharing。

---

### 11. 修复方案验证

**问题**: 对比三种 False Sharing 修复方案的性能：

1. `alignas(64)`
2. 手动填充 `padding[56]`
3. `CacheLineAligned<T>` 模板

---

### 12. 内核配置

**问题**: 检查你的内核是否支持 perf c2c 所需的事件：
```bash
cat /proc/sys/kernel/perf_event_paranoid
```

解释不同值 (0, 1, 2) 的影响。