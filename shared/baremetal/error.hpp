// shared/baremetal/error.hpp
// ARM64 裸机编程 - 错误码定义
// 参考 ncnn 项目风格

#pragma once

namespace benos {

// 错误码定义 (参考 ncnn)
// 0 表示成功，负数表示错误
enum class Error : int {
    OK = 0,
    Success = 0,

    // 通用错误
    Unknown = -1,
    InvalidParam = -2,
   OutOfMemory = -3,
    BufferOverflow = -4,

    // 硬件错误
    HardwareError = -100,
    UartError = -101,
    TimerError = -102,
    GicError = -103,

    // 系统错误
    NotInitialized = -200,
    AlreadyInitialized = -201,
};

// 判断宏 (参考 ncnn)
#define BENOS_OK(result)    ((result) >= 0)
#define BENOS_FAILED(result) ((result) < 0)

// 便捷函数
inline auto is_ok(int result) -> bool { return result >= 0; }
inline auto is_failed(int result) -> bool { return result < 0; }

} // namespace benos