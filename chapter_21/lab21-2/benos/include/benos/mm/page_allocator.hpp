// include/benos/mm/page_allocator.hpp
#pragma once

#include <benos/types.hpp>

namespace benos {

class PageAllocator {
public:
    static constexpr usize PAGE_SIZE = 4096;

    void init(uptr start, uptr end);
    auto alloc() -> void*;
    void free(void* page);

    auto total_pages() const -> usize;
    auto free_pages() const -> usize;

    static auto instance() -> PageAllocator&;

private:
    PageAllocator() = default;
};

extern PageAllocator& g_page_allocator;

} // namespace benos

// C compatibility - interface with existing code
extern "C" {
    void mem_init(unsigned long start_mem, unsigned long end_mem);
    unsigned long get_free_page(void);
    void free_page(unsigned long page);
}