// kernel/process_manager.cpp - Process manager C++ wrapper implementation
#include <benos/kernel/process.hpp>
#include <sched.h>

namespace benos {

// Meyers Singleton - instance created inside instance() function
auto ProcessManager::instance() -> ProcessManager& {
    static ProcessManager process_manager_instance;
    return process_manager_instance;
}

// Global reference - initialize on first use
ProcessManager& g_process_manager = ProcessManager::instance();

auto ProcessManager::create_process(void (*fn)(), u64 arg) -> i32 {
    // Call the C do_fork function
    return do_fork(static_cast<unsigned long>(ProcessFlags::KThread),
                   reinterpret_cast<unsigned long>(fn),
                   static_cast<unsigned long>(arg));
}

auto ProcessManager::find_by_pid(i32 pid) -> task_struct* {
    if (pid < 0 || pid >= NR_TASK) {
        return nullptr;
    }
    return g_task[pid];
}

// Process class implementations
auto Process::state() const -> ProcessState {
    // This would need to access the underlying task_struct
    // For now, return a default value since this is a stub
    return ProcessState::Running;
}

auto Process::pid() const -> i32 {
    // Stub implementation
    return -1;
}

auto Process::priority() const -> i32 {
    // Stub implementation
    return 1;
}

} // namespace benos
