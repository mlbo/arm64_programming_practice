// uart.hpp - UART driver for PL011
#pragma once

#include "types.hpp"
#include "io.hpp"

namespace arm64lab {

#ifdef CONFIG_BOARD_PI3B
constexpr uptr PBASE = 0x3F000000;
#else
constexpr uptr PBASE = 0xFE000000;
#endif

constexpr uptr U_BASE = PBASE + 0x00201000;
constexpr uptr U_DATA_REG = U_BASE;
constexpr uptr U_FR_REG   = U_BASE + 0x18;
constexpr uptr U_IBRD_REG = U_BASE + 0x24;
constexpr uptr U_FBRD_REG = U_BASE + 0x28;
constexpr uptr U_LCRH_REG = U_BASE + 0x2C;
constexpr uptr U_CR_REG   = U_BASE + 0x30;
constexpr uptr U_IMSC_REG = U_BASE + 0x38;

constexpr uptr GPFSEL1 = PBASE + 0x00200004;
constexpr uptr GPPUD   = PBASE + 0x00200094;
constexpr uptr GPPUDCLK0 = PBASE + 0x00200098;
constexpr uptr GPIO_PUP_PDN_CNTRL_REG0 = PBASE + 0x002000E4;

class UartDriver {
public:
    void init();
    void send(char c);
    auto recv() -> char;
    void send_string(const char* str);
    void send_hex(u32 value);
    static auto instance() -> UartDriver&;
private:
    UartDriver() = default;
    bool initialized_ = false;
};

extern UartDriver& g_uart;

} // namespace arm64lab