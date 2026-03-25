// include/benos/error.hpp
#pragma once

#include "types.hpp"

namespace benos {

enum class Error : i32 {
    OK = 0,
    Unknown = -1,
    NoMemory = -2,
    InvalidParam = -3,
};

constexpr auto success(i32 result) -> bool { return result >= 0; }
constexpr auto failed(i32 result) -> bool { return result < 0; }

} // namespace benos