// include/benos/kernel/process.hpp
#pragma once

#include <benos/types.hpp>

// Forward declaration of C structs
struct task_struct;

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
    // This class wraps the C task_struct, doesn't own data
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

// C compatibility - interface with existing scheduler code
extern "C" {
    int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg);
    void switch_to(struct task_struct *next);

    extern struct task_struct *g_task[];
    extern union task_union init_task_union;
}

// Legacy C struct definitions from sched.h
#ifndef NR_TASK
#define NR_TASK 128
#endif

#ifndef THREAD_SIZE
#define THREAD_SIZE (1 * 4096)  // PAGE_SIZE
#define THREAD_START_SP (THREAD_SIZE - 8)
#endif

struct cpu_context {
    unsigned long x19;
    unsigned long x20;
    unsigned long x21;
    unsigned long x22;
    unsigned long x23;
    unsigned long x24;
    unsigned long x25;
    unsigned long x26;
    unsigned long x27;
    unsigned long x28;
    unsigned long fp;
    unsigned long sp;
    unsigned long pc;
};

union task_union {
    struct task_struct task;
    unsigned long stack[THREAD_SIZE/sizeof(long)];
};