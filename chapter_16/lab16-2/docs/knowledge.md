# perf c2c 工具知识点

## 1. perf 工具概述

perf 是 Linux 内核性能分析工具，可以监控 CPU、缓存、内存等各种硬件事件。

### 常用子命令
- `perf record`: 采样记录
- `perf report`: 分析报告
- `perf stat`: 统计事件
- `perf c2c`: 缓存行竞争分析

## 2. perf c2c 原理

`perf c2c` (Cache-to-Cache) 利用 CPU 的性能监控单元 (PMU) 追踪：
- 内存访问事件
- 缓存一致性流量
- 缓存行状态变化

### 支持的事件
```
cpu/mem-loads,ldlat=30/P  # 内存加载事件
cpu/mem-stores/P          # 内存存储事件
```

## 3. 关键指标解释

### HITM (Hit Modified)
当 CPU 请求的数据在另一个 CPU 的缓存中且处于 Modified 状态：
- 发生缓存行传递
- 表示缓存行竞争
- **高 HITM = 可能存在 False Sharing**

### LLC Misses (Last Level Cache)
最后一级缓存缺失：
- 数据不在任何缓存中
- 需要从内存获取

### Shared Cache Lines
被多个 CPU 访问的缓存行：
- 正常共享是预期的
- 高频写入共享行 = 问题

## 4. perf c2c 报告分析

### 报告结构
```
=================================================
            Trace Event Information
=================================================
  Total records                     :     177504
  Load Operations                   :      76801
  Store Operations                  :     100703
  ...
```

### 关键分析步骤

1. **查看总览统计**
   ```
   perf c2c report --stats
   ```

2. **查看共享缓存行**
   ```
   perf c2c report
   ```
   按 `s` 按内存地址排序

3. **识别热点**
   - 高 HITM 的地址
   - 多个 CPU 访问同一地址

## 5. 实际案例

### lab16-2 输出示例
```
=================================================
    Global Shared Cache Line Event Information
=================================================
  Total Shared Cache Lines          :          1
  Load HITs on shared lines         :      42461
  Store HITs on shared lines        :      38640
```

**解读**: 只有 1 个共享缓存行，但访问量极高，典型 False Sharing。

## 6. 使用技巧

### 采样频率
```bash
# 提高采样频率 (默认 4000)
perf c2c record -F 10000 ./program
```

### 过滤特定 CPU
```bash
# 只监控 CPU 0-3
perf c2c record -C 0-3 ./program
```

### 输出到文件
```bash
perf c2c record -o perf.data ./program
perf c2c report -i perf.data
```

## 7. 其他分析工具

### Intel VTune
- 图形界面
- 更详细的分析
- 商业软件

### perf mem
```bash
perf mem record ./program
perf mem report
```

### Valgrind cachegrind
```bash
valgrind --tool=cachegrind ./program
cg_annotate cachegrind.out.*
```