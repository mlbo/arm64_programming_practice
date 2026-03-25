// include/benos/driver/uart.hpp
#pragma once

#include "benos/types.hpp"

namespace benos {

class UartDriver {
public:
    void init();
    void send(char c);
    auto recv() -> char;
    void send_string(const char* str);

    static auto instance() -> UartDriver&;

private:
    UartDriver() = default;
    bool initialized_ = false;
};

extern UartDriver& g_uart;

} // namespace benos

// C compatibility - 调用现有 C 实现
extern "C" {
    void uart_init(void);
    void uart_send(char c);
    char uart_recv(void);
    void uart_send_string(char *str);
}