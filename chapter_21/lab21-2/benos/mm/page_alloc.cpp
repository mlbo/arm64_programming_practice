// mm/page_alloc.cpp
// Page allocator implementation - C++ wrapper around original C logic

#include <benos/mm/page_allocator.hpp>
#include <printk.h>
#include <asm/mm.h>

// Using original static array and logic
#define NR_PAGES (TOTAL_MEMORY / PAGE_SIZE)

static unsigned short mem_map[NR_PAGES] = {0,};
static unsigned long phy_start_address;

namespace benos {

void PageAllocator::init(uptr start, uptr end) {
    // Delegate to C implementation
    ::mem_init(start, end);
}

auto PageAllocator::alloc() -> void* {
    return reinterpret_cast<void*>(::get_free_page());
}

void PageAllocator::free(void* page) {
    ::free_page(reinterpret_cast<unsigned long>(page));
}

auto PageAllocator::total_pages() const -> usize {
    return NR_PAGES;
}

auto PageAllocator::free_pages() const -> usize {
    usize count = 0;
    for (usize i = 0; i < NR_PAGES; ++i) {
        if (mem_map[i] == 0) count++;
    }
    return count;
}

auto PageAllocator::instance() -> PageAllocator& {
    static PageAllocator allocator;
    return allocator;
}

PageAllocator& g_page_allocator = PageAllocator::instance();

} // namespace benos

// Original C implementation - preserved for compatibility
extern "C" {

void mem_init(unsigned long start_mem, unsigned long end_mem)
{
    unsigned long nr_free_pages = 0;
    unsigned long free;

    start_mem = PAGE_ALIGN(start_mem);
    phy_start_address = start_mem;
    end_mem &= PAGE_MASK;
    free = end_mem - start_mem;

    while (start_mem < end_mem) {
        nr_free_pages++;
        start_mem += PAGE_SIZE;
    }

    printk("Memory: %uKB available, %u free pages\n", free/1024, nr_free_pages);
}

unsigned long get_free_page(void)
{
    int i;

    for (i = 0; i < NR_PAGES; i++) {
        if (mem_map[i] == 0) {
            mem_map[i] = 1;
            return LOW_MEMORY + i * PAGE_SIZE;
        }
    }
    return 0;
}

void free_page(unsigned long p)
{
    mem_map[(p - LOW_MEMORY)/PAGE_SIZE] = 0;
}

} // extern "C"