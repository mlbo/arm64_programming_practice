// lib/timer_wrapper.cpp
#include <benos/driver/timer.hpp>

namespace benos {

void TimerDriver::init() {
    ::timer_init();
}

void TimerDriver::handle_irq() {
    ::handle_timer_irq();
}

void TimerDriver::reset() {
    // TODO: Implement timer reset
}

auto TimerDriver::instance() -> TimerDriver& {
    static TimerDriver timer;
    return timer;
}

TimerDriver& g_timer = TimerDriver::instance();

} // namespace benos