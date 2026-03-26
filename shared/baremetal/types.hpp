// shared/baremetal/types.hpp
// ARM64 裸机编程 - 基础类型定义
// 参考 ncnn 项目风格
// 使用编译器内置类型，不依赖标准库

#pragma once

// Common constants for both C++ and assembly
#ifndef __ASSEMBLY__

// 使用 GCC/Clang 内置类型定义 (freestanding 兼容)
namespace benos {

// 无符号整数类型
using u8   = __UINT8_TYPE__;
using u16  = __UINT16_TYPE__;
using u32  = __UINT32_TYPE__;
using u64  = __UINT64_TYPE__;
using usize = __SIZE_TYPE__;
using uptr = __UINTPTR_TYPE__;

// 有符号整数类型
using i8   = __INT8_TYPE__;
using i16  = __INT16_TYPE__;
using i32  = __INT32_TYPE__;
using i64  = __INT64_TYPE__;
using isize = __PTRDIFF_TYPE__;

// 指针类型
using byte = unsigned char;

// 空指针常量
constexpr auto nullptr_v = nullptr;

} // namespace benos

// 简化类型别名 (不带命名空间，用于 C 兼容)
using benos_u8   = benos::u8;
using benos_u16  = benos::u16;
using benos_u32  = benos::u32;
using benos_u64  = benos::u64;
using benos_usize = benos::usize;
using benos_i8   = benos::i8;
using benos_i16  = benos::i16;
using benos_i32  = benos::i32;
using benos_i64  = benos::i64;

#endif // __ASSEMBLY__