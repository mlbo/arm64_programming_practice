// include/benos/kernel/process.hpp
#pragma once

#include <benos/types.hpp>

// Forward declarations only - do not include C headers here
struct task_struct;
union task_union;

namespace benos {

// Process flags (matches C enum task_flags)
enum class ProcessFlags : u32 {
    KThread = 1 << 0,
};

// Process state (matches C enum task_state)
enum class ProcessState : u32 {
    Running = 0,
    Interruptible = 1,
    Uninterruptible = 2,
    Zombie = 3,
    Stopped = 4,
};

// C++ wrapper for task_struct
class Process {
public:
    Process() = default;

    auto state() const -> ProcessState;
    auto pid() const -> i32;
    auto priority() const -> i32;

private:
    // This class wraps the C task_struct, does not own data
};

// Process manager - singleton that wraps C scheduler functions
class ProcessManager {
public:
    // Create a new kernel thread, returns pid on success, negative on error
    auto create_process(void (*fn)(), u64 arg = 0) -> i32;

    // Find process by pid
    auto find_by_pid(i32 pid) -> task_struct*;

    static auto instance() -> ProcessManager&;

private:
    ProcessManager() = default;
};

extern ProcessManager& g_process_manager;

} // namespace benos

// C compatibility declarations
extern "C" {
    int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg);
    void switch_to(struct task_struct *next);

    extern struct task_struct *g_task[];
    extern union task_union init_task_union;
}
