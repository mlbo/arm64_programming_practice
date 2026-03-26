// pl_uart.cpp - PL011 UART driver implementation
#include "uart.hpp"

namespace arm64lab {

auto UartDriver::instance() -> UartDriver& {
    static UartDriver inst;
    return inst;
}

// Global UART instance reference
UartDriver& g_uart = UartDriver::instance();

void UartDriver::init() {
    if (initialized_) {
        return;
    }

    u32 selector;

    // Clean gpio14
    selector = io::readl(GPFSEL1);
    selector &= ~(7 << 12);
    // Set alt0 for gpio14
    selector |= 4 << 12;
    // Clean gpio15
    selector &= ~(7 << 15);
    // Set alt0 for gpio15
    selector |= 4 << 15;
    io::writel(selector, GPFSEL1);

#ifdef CONFIG_BOARD_PI3B
    io::writel(0, GPPUD);
    io::delay(150);
    io::writel((1 << 14) | (1 << 15), GPPUDCLK0);
    io::delay(150);
    io::writel(0, GPPUDCLK0);
#else
    // Set gpio14/15 pull down state
    selector = io::readl(GPIO_PUP_PDN_CNTRL_REG0);
    selector |= (0x2 << 30) | (0x2 << 28);
    io::writel(selector, GPIO_PUP_PDN_CNTRL_REG0);
#endif

    // Disable UART until configuration is done
    io::writel(0, U_CR_REG);

    /*
     * Baud divisor = UARTCLK / (16 * baud_rate)
     * = 48 * 10^6 / (16 * 115200) = 26.0416666667
     * Integer part = 26
     * Fractional part = (int)((0.0416666667 * 64) + 0.5) = 3
     * Generated baud rate divisor = 26 + (3 / 64) = 26.046875
     * Generated baud rate = (48 * 10^6) / (16 * 26.046875) = 115177
     * Error = |(115177 - 115200) / 115200 * 100| = 0.02%
     */

    // Baud rate divisor, integer part
    io::writel(26, U_IBRD_REG);
    // Baud rate divisor, fractional part
    io::writel(3, U_FBRD_REG);

    // Enable FIFOs and 8 bits frames
    io::writel((1 << 4) | (3 << 5), U_LCRH_REG);

    // Mask interrupts
    io::writel(0, U_IMSC_REG);

    // Enable UART, receive and transmit
    io::writel(1 | (1 << 8) | (1 << 9), U_CR_REG);

    initialized_ = true;
}

void UartDriver::send(char c) {
    // Wait for transmit FIFO to have an available slot
    while (io::readl(U_FR_REG) & (1 << 5)) {
        ;
    }
    io::writel(static_cast<u32>(c), U_DATA_REG);
}

auto UartDriver::recv() -> char {
    // Wait for receive FIFO to have data to read
    while (io::readl(U_FR_REG) & (1 << 4)) {
        ;
    }
    return static_cast<char>(io::readl(U_DATA_REG) & 0xFF);
}

void UartDriver::send_string(const char* str) {
    while (*str) {
        send(*str++);
    }
}

} // namespace arm64lab