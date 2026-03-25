// lib/uart_wrapper.cpp
#include <benos/driver/uart.hpp>

namespace benos {

void UartDriver::init() {
    if (initialized_) return;
    ::uart_init();  // 调用现有 C 实现
    initialized_ = true;
}

void UartDriver::send(char c) { ::uart_send(c); }

auto UartDriver::recv() -> char { return ::uart_recv(); }

void UartDriver::send_string(const char* str) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    ::uart_send_string(const_cast<char*>(str));
}

auto UartDriver::instance() -> UartDriver& {
    static UartDriver uart;
    return uart;
}

UartDriver& g_uart = UartDriver::instance();

} // namespace benos