// UART driver for Raspberry Pi
// Supports both PL011 (UART0) and mini UART

#pragma once

#include "../../shared/baremetal/types.hpp"

namespace benos {

// UART base addresses
enum : uptr {
    UART0_BASE = 0xFE201000,  // PL011 UART (Pi4)
    UART0_BASE_PI3 = 0x3F201000,  // PL011 UART (Pi3)
};

// UART class
class Uart {
public:
    static auto instance() -> Uart& {
        static Uart inst;
        return inst;
    }

    void init();
    void send(char c);
    auto recv() -> char;
    void send_string(const char* str);

    // Polling helpers
    auto recv_ready() -> bool;
    auto send_ready() -> bool;

private:
    Uart() = default;

    void init_pl011();
    void init_gpio();

    volatile u32* regs_ = nullptr;
    bool initialized_ = false;
};

extern Uart& g_uart;

}  // namespace benos