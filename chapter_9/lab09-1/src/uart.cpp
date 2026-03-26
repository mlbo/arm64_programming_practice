// UART driver implementation
// Raspberry Pi PL011 UART

#include "uart.hpp"

#ifdef CONFIG_BOARD_PI3B
#define UART_BASE UART0_BASE_PI3
#else
#define UART_BASE UART0_BASE
#endif

namespace benos {

// PL011 register offsets
enum : u32 {
    UART_DR   = 0x00,  // Data Register
    UART_FR   = 0x18,  // Flag Register
    UART_IBRD = 0x24,  // Integer Baud Rate
    UART_FBRD = 0x28,  // Fractional Baud Rate
    UART_LCRH = 0x2C,  // Line Control
    UART_CR   = 0x30,  // Control Register
    UART_IMSC = 0x38,  // Interrupt Mask
    UART_ICR  = 0x44,  // Interrupt Clear
};

// Flag Register bits
enum : u32 {
    FR_TXFE = 1 << 5,  // Transmit FIFO empty
    FR_RXFF = 1 << 4,  // Receive FIFO full
};

// GPIO registers (for UART pins)
enum : uptr {
    GPIO_BASE = 0xFE200000,
    GPFSEL1   = 0x04,
    GPPUD     = 0x94,
    GPPUDCLK0 = 0x98,
};

// Global UART reference
Uart& g_uart = Uart::instance();

void Uart::init() {
    if (initialized_) return;

    regs_ = reinterpret_cast<volatile u32*>(UART_BASE);
    init_gpio();
    init_pl011();

    initialized_ = true;
}

void Uart::init_gpio() {
    volatile u32* gpio = reinterpret_cast<volatile u32*>(GPIO_BASE);

    // Set GPIO14 and GPIO15 to ALT0 (UART0)
    gpio[GPFSEL1 / 4] = (gpio[GPFSEL1 / 4] & ~(7 << 12)) | (4 << 12);  // GPIO14
    gpio[GPFSEL1 / 4] = (gpio[GPFSEL1 / 4] & ~(7 << 15)) | (4 << 15);  // GPIO15

    // Disable pull-up/down
    gpio[GPPUD / 4] = 0;
    for (volatile int i = 0; i < 150; ++i) asm volatile("nop");
    gpio[GPPUDCLK0 / 4] = (1 << 14) | (1 << 15);
    for (volatile int i = 0; i < 150; ++i) asm volatile("nop");
    gpio[GPPUDCLK0 / 4] = 0;
}

void Uart::init_pl011() {
    // Disable UART
    regs_[UART_CR / 4] = 0;

    // Wait for TX to complete
    while (regs_[UART_FR / 4] & FR_TXFE) asm volatile("nop");

    // Disable interrupts
    regs_[UART_IMSC / 4] = 0;

    // Set baud rate to 115200
    // CLK = 48MHz, BAUDDIV = 48000000 / (16 * 115200) = 26.0416
    regs_[UART_IBRD / 4] = 26;   // Integer part
    regs_[UART_FBRD / 4] = 3;    // Fractional part (0.0416 * 64 = 2.67 ≈ 3)

    // Set 8N1 format
    regs_[UART_LCRH / 4] = (3 << 5);  // 8 bits, no parity, 1 stop bit

    // Enable UART
    regs_[UART_CR / 4] = (1 << 0) | (1 << 8) | (1 << 9);  // Enable, TX, RX
}

void Uart::send(char c) {
    // Wait for TX FIFO to have space
    while (regs_[UART_FR / 4] & FR_TXFE) {
        asm volatile("nop");
    }
    regs_[UART_DR / 4] = c;
}

auto Uart::recv() -> char {
    // Wait for RX FIFO to have data
    while (regs_[UART_FR / 4] & FR_RXFF) {
        asm volatile("nop");
    }
    return static_cast<char>(regs_[UART_DR / 4] & 0xFF);
}

void Uart::send_string(const char* str) {
    while (*str) {
        if (*str == '\n') {
            send('\r');
        }
        send(*str++);
    }
}

auto Uart::recv_ready() -> bool {
    return !(regs_[UART_FR / 4] & FR_RXFF);
}

auto Uart::send_ready() -> bool {
    return !(regs_[UART_FR / 4] & FR_TXFE);
}

}  // namespace benos