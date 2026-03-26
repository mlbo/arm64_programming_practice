

#ifndef PAGE_SHIFT
#define PAGE_SHIFT 12
#endif

#ifndef TABLE_SHIFT
#define TABLE_SHIFT 9
#endif

#ifndef SECTION_SHIFT
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)
#endif


#ifndef PAGE_SIZE
#define PAGE_SIZE (1 << PAGE_SHIFT)
#endif

#ifndef SECTION_SIZE
#define SECTION_SIZE (1 << SECTION_SHIFT)
#endif

#define PAGE_MASK (~(PAGE_SIZE-1))
/* to align the pointer to the (next) page boundary */
#define PAGE_ALIGN(addr) (((addr)+PAGE_SIZE-1)&PAGE_MASK)

