// shared/baremetal/runtime/cxx_runtime.cpp
// ARM64 裸机编程 - C++ 运行时支持
// 提供 pure virtual 和 guardian 函数的实现

#include <stdint.h>

// Pure virtual call handler
extern "C" void __cxa_pure_virtual() {
    // 在裸机环境中，pure virtual 调用是致命错误
    // 可以在这里添加错误处理或无限循环
    while (1) {
        asm volatile("wfe");
    }
}

// Deleted virtual function handler
extern "C" void __cxa_deleted_virtual() {
    while (1) {
        asm volatile("wfe");
    }
}

// 如果需要完整的 C++ 异常支持 (通常裸机环境禁用)
// 这些函数在 -fno-exceptions 时不需要
#if !defined(__EXCEPTIONS) || !__EXCEPTIONS
// 占位符
#endif