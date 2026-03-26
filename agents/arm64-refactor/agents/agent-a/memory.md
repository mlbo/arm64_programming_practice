# agent-a 操作记录

## 记录格式
- `YYYY-MM-DD HH:MM:SS +0800`：操作描述

---

## 操作历史

### 2026-03-26 15:30:00 +0800

**状态**: done

**任务**: CH2-4 重构完成

**变更说明**:

#### Chapter 2: Hello BenOS
- 重构 `chapter_2/lab01_hello_benos/BenOS/` 到 C++
- 创建 C++ 头文件: `types.hpp`, `io.hpp`, `mm.hpp`, `uart.hpp`
- 创建 C++ 实现: `pl_uart.cpp`, `kernel.cpp`
- 更新 `boot.S` 和 `Makefile` 支持 C++ 编译
- 添加完整文档: `README.md`, `docs/knowledge.md`, `docs/code_explanation.md`, `docs/exercises.md`
- 添加脚本: `scripts/build.sh`, `scripts/run.sh`

#### Chapter 3: LDR/STR
- 重构 6 个 labs (lab03-1 ~ lab03-6)
- lab03-1: LDR 偏移模式
- lab03-2: 前/后变基寻址
- lab03-3: STR 前/后变基
- lab04-4: Label 访问
- lab03-5: memcpy 实现
- lab03-6: memset 实现 (含 C++ 和汇编优化)

#### Chapter 4: 数据处理指令
- 重构 5 个 labs (lab04-1 ~ lab04-5)
- lab04-1: Label 访问测试
- lab04-2: ADDS/CMP/ADC 指令
- lab04-3: CMP/SBC 指令
- lab04-4: ANDS 与 Z 标志位
- lab04-5: BFI/UBFX/SBFX 位段指令 (含 `bit_ops.hpp` 工具类)

**验证要求**:
需要安装 `aarch64-linux-gnu-g++` 和 `qemu-system-aarch64` 进行验证。