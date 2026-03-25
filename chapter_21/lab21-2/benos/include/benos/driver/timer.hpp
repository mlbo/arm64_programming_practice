// include/benos/driver/timer.hpp
#pragma once

#include "benos/types.hpp"

namespace benos {

class TimerDriver {
public:
    void init();
    void handle_irq();
    void reset();

    static auto instance() -> TimerDriver&;

private:
    TimerDriver() = default;
};

extern TimerDriver& g_timer;

} // namespace benos

// C compatibility
extern "C" {
    void timer_init(void);
    void handle_timer_irq(void);
}