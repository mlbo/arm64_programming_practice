// include/arch/arm64/cache.hpp
// ARM64 缓存管理
// 参考 ncnn 项目风格

#pragma once

#include <stdint.h>

namespace benos::arch::arm64 {

/// 缓存信息结构
struct CacheInfo {
    u32 line_size;      ///< 缓存行大小 (字节)
    u32 sets;           ///< 组数
    u32 ways;           ///< 路数
    u32 level;          ///< 缓存级别 (1-7)
    u32 size_kb;        ///< 缓存大小 (KB)
    bool is_instruction; ///< 是否为指令缓存
};

/// 缓存类型枚举
enum class CacheType : u32 {
    None     = 0,  ///< 无缓存
    Instr    = 1,  ///< 仅指令缓存
    Data     = 2,  ///< 仅数据缓存
    Separate = 3,  ///< 分离缓存 (I$ + D$)
    Unified  = 4,  ///< 统一缓存
};

/// 缓存维护类
///
/// 提供缓存信息查询和维护操作。
///
/// 使用示例:
/// ```cpp
/// auto info = CacheMaintainer::get_cache_info(1, false);
/// printk("L1 D-Cache: %uKB\n", info.size_kb);
///
/// CacheMaintainer::clean_and_invalidate_by_va(addr);
/// ```
class CacheMaintainer {
public:
    /// 获取缓存信息
    /// @param level 缓存级别 (1-7)
    /// @param is_instruction true=指令缓存, false=数据/统一缓存
    /// @return 缓存信息结构
    static auto get_cache_info(u32 level, bool is_instruction) -> CacheInfo;

    /// 获取缓存类型
    /// @param level 缓存级别
    /// @return 缓存类型
    static auto get_cache_type(u32 level) -> CacheType;

    /// 按 VA clean 数据缓存
    static void clean_by_va(u64 va);

    /// 按 VA invalidate 数据缓存
    static void invalidate_by_va(u64 va);

    /// 按 VA clean and invalidate 数据缓存
    static void clean_and_invalidate_by_va(u64 va);

    /// 按 VA clean 到 PoU (用于代码修改)
    static void clean_by_va_to_pou(u64 va);

    /// 按组路 clean 整个数据缓存
    static void clean_all(u32 level);

    /// 按组路 invalidate 整个数据缓存
    static void invalidate_all(u32 level);

    /// 按 set/way clean and invalidate
    static void clean_and_invalidate_all(u32 level);

    /// 刷新整个指令缓存
    static void invalidate_icache_all();

    /// 获取数据缓存行大小
    static auto get_dcache_line_size() -> u32;

    /// 获取指令缓存行大小
    static auto get_icache_line_size() -> u32;

    /// 获取最大缓存级别
    static auto get_max_cache_level() -> u32;

private:
    static auto read_ccsidr(u32 level, bool is_instruction) -> u64;
};

/// 页表遍历器
class PageTableWalker {
public:
    /// 遍历页表，返回物理地址
    /// @param pgd 页表基地址
    /// @param va 虚拟地址
    /// @return 物理地址，0 表示未映射
    static auto walk(u64* pgd, u64 va) -> u64;

    /// 获取物理地址 (别名)
    static auto get_pa(u64* pgd, u64 va) -> u64 {
        return walk(pgd, va);
    }
};

} // namespace benos::arch::arm64

// ============================================================================
// 缓存寄存器定义
// ============================================================================

namespace benos::cache {

// CTR_EL0 位域
constexpr u32 CTR_L1IP_SHIFT = 14;
constexpr u32 CTR_L1IP_MASK  = 3;
constexpr u32 CTR_DMINLINE_SHIFT = 16;
constexpr u32 CTR_IMINLINE_SHIFT = 0;
constexpr u32 CTR_ERG_SHIFT = 20;
constexpr u32 CTR_CWG_SHIFT = 24;
constexpr u32 CTR_CWG_MASK  = 15;

// 指令缓存策略
constexpr u32 ICACHE_POLICY_VPIPT = 0;
constexpr u32 ICACHE_POLICY_AIVIVT = 1;
constexpr u32 ICACHE_POLICY_VIPT = 2;
constexpr u32 ICACHE_POLICY_PIPT = 3;

// CLIDR_EL1 位域
constexpr u32 CLIDR_ICB_SHIFT  = 30;
constexpr u32 CLIDR_LOUU_SHIFT = 27;
constexpr u32 CLIDR_LOC_SHIFT  = 24;
constexpr u32 CLIDR_LOUIS_SHIFT = 21;

// CCSIDR_EL1 位域
constexpr u32 CCSIDR_NUMSETS_SHIFT = 13;
constexpr u32 CCSIDR_NUMSETS_MASK  = (0x1FFF << CCSIDR_NUMSETS_SHIFT);
constexpr u32 CCSIDR_ASSOC_SHIFT   = 3;
constexpr u32 CCSIDR_ASSOC_MASK    = (0x3FF << CCSIDR_ASSOC_SHIFT);
constexpr u32 CCSIDR_LINESIZE_MASK = 0x7;

// CSSELR_EL1 位域
constexpr u32 CSSELR_IND_I = (1 << 0);  // 指令缓存标志
constexpr u32 CSSELR_LEVEL_SHIFT = 1;

// 最大缓存级别
constexpr u32 MAX_CACHE_LEVEL = 7;

} // namespace benos::cache