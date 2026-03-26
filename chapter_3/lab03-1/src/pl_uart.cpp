// pl_uart.cpp - PL011 UART driver implementation
#include "uart.hpp"

namespace arm64lab {

auto UartDriver::instance() -> UartDriver& {
    static UartDriver inst;
    return inst;
}

UartDriver& g_uart = UartDriver::instance();

void UartDriver::init() {
    if (initialized_) return;

    u32 selector;
    selector = io::readl(GPFSEL1);
    selector &= ~(7 << 12);
    selector |= 4 << 12;
    selector &= ~(7 << 15);
    selector |= 4 << 15;
    io::writel(selector, GPFSEL1);

#ifdef CONFIG_BOARD_PI3B
    io::writel(0, GPPUD);
    io::delay(150);
    io::writel((1 << 14) | (1 << 15), GPPUDCLK0);
    io::delay(150);
    io::writel(0, GPPUDCLK0);
#else
    selector = io::readl(GPIO_PUP_PDN_CNTRL_REG0);
    selector |= (0x2 << 30) | (0x2 << 28);
    io::writel(selector, GPIO_PUP_PDN_CNTRL_REG0);
#endif

    io::writel(0, U_CR_REG);
    io::writel(26, U_IBRD_REG);
    io::writel(3, U_FBRD_REG);
    io::writel((1 << 4) | (3 << 5), U_LCRH_REG);
    io::writel(0, U_IMSC_REG);
    io::writel(1 | (1 << 8) | (1 << 9), U_CR_REG);

    initialized_ = true;
}

void UartDriver::send(char c) {
    while (io::readl(U_FR_REG) & (1 << 5)) {}
    io::writel(static_cast<u32>(c), U_DATA_REG);
}

auto UartDriver::recv() -> char {
    while (io::readl(U_FR_REG) & (1 << 4)) {}
    return static_cast<char>(io::readl(U_DATA_REG) & 0xFF);
}

void UartDriver::send_string(const char* str) {
    while (*str) send(*str++);
}

void UartDriver::send_hex(u32 value) {
    const char hex[] = "0123456789ABCDEF";
    char buf[9];
    for (int i = 7; i >= 0; --i) {
        buf[i] = hex[value & 0xF];
        value >>= 4;
    }
    buf[8] = '\0';
    send_string(buf);
}

} // namespace arm64lab