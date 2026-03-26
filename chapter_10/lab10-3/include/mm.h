#ifndef	_MM_H
#define	_MM_H


#ifndef PAGE_SHIFT
#define PAGE_SHIFT	 		12
#endif

#ifndef TABLE_SHIFT
#define TABLE_SHIFT 			9
#endif

#ifndef SECTION_SHIFT
#define SECTION_SHIFT			(PAGE_SHIFT + TABLE_SHIFT)
#endif


#ifndef PAGE_SIZE
#define PAGE_SIZE   			(1 << PAGE_SHIFT)	
#endif

#ifndef SECTION_SIZE
#define SECTION_SIZE			(1 << SECTION_SHIFT)	
#endif

#define LOW_MEMORY              	(2 * SECTION_SIZE)

#ifndef __ASSEMBLER__

void memzero(unsigned long src, unsigned long n);

#endif

#endif  /*_MM_H */
