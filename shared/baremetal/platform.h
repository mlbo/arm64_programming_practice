// shared/baremetal/platform.h
// ARM64 裸机编程 - 平台检测宏

#pragma once

// 编译器检测
#if defined(__GNUC__)
    #define BENOS_COMPILER_GCC 1
    #define BENOS_COMPILER_CLANG 0
#elif defined(__clang__)
    #define BENOS_COMPILER_GCC 0
    #define BENOS_COMPILER_CLANG 1
#else
    #define BENOS_COMPILER_GCC 0
    #define BENOS_COMPILER_CLANG 0
#endif

// 架构检测
#if defined(__aarch64__)
    #define BENOS_ARCH_ARM64 1
    #define BENOS_ARCH_ARM32 0
#elif defined(__arm__)
    #define BENOS_ARCH_ARM64 0
    #define BENOS_ARCH_ARM32 1
#else
    #define BENOS_ARCH_ARM64 0
    #define BENOS_ARCH_ARM32 0
#endif

// 平台检测
#if defined(CONFIG_BOARD_PI4B)
    #define BENOS_BOARD_RPI4 1
    #define BENOS_BOARD_RPI3 0
#elif defined(CONFIG_BOARD_PI3B)
    #define BENOS_BOARD_RPI4 0
    #define BENOS_BOARD_RPI3 1
#else
    #define BENOS_BOARD_RPI4 0
    #define BENOS_BOARD_RPI3 0
#endif

// 调试宏
#ifdef DEBUG
    #define BENOS_DEBUG 1
#else
    #define BENOS_DEBUG 0
#endif

// 内联属性
#if BENOS_COMPILER_GCC || BENOS_COMPILER_CLANG
    #define BENOS_INLINE static inline __attribute__((always_inline))
    #define BENOS_NORETURN __attribute__((noreturn))
    #define BENOS_PACKED __attribute__((packed))
    #define BENOS_ALIGNED(x) __attribute__((aligned(x)))
#else
    #define BENOS_INLINE static inline
    #define BENOS_NORETURN
    #define BENOS_PACKED
    #define BENOS_ALIGNED(x)
#endif

// 通用寄存器操作宏 (用于启动代码)
#if BENOS_ARCH_ARM64
    // 读取系统寄存器
    #define BENOS_READ_SYSREG(reg) ({ \
        u64 __val; \
        asm volatile("mrs %0, " #reg : "=r"(__val)); \
        __val; \
    })

    // 写入系统寄存器
    #define BENOS_WRITE_SYSREG(reg, val) ({ \
        asm volatile("msr " #reg ", %0" :: "r"(val)); \
    })
#endif