/**
**	Name: page_table.C
**	Author: Pranami Bhattacharya
**	UIN: 223004200
**	Desc: Implementation of basic paging
**	Date: 02/14/14
**/

/* Flow of the algorithm
	create page table-> init_paging
	load into processor context-> load()
	switch from physical to logical addressing-> enable_paging()
	handle page fault and store the address-> handle_fault() 
*/

/* DEFINES */
#define KB 	* (0x1 << 10) // 2^10 bytes
#define MB	* (0x1 << 20) // 2^20 bytes
#define MASK 		0x3FF;
#define PG_SIZE 	4096
    	   
#include "page_table.H"

/*Setting up the page directory */
/* kernel data may be written between 2MB and 4MB */
unsigned long *PageTable :: page_directory = (unsigned long *) 0x200000; // start addr of the page dir;
									// decimal 2*1024*1024 bytes
/* Setting up the page table */
unsigned long *PageTable :: page_table = (unsigned long *) 0x201000 ; /*
								page table comes after page dir at the next
								4 KB boundary */
/* declare variables */
unsigned long PageTable :: shared_size; 
PageTable *PageTable :: current_page_table;
FramePool *PageTable :: kernel_mem_pool;
FramePool *PageTable :: process_mem_pool;
unsigned int PageTable :: paging_enabled;
unsigned long PageTable :: address;
PageTable :: PageTable()
{
}
								
/* fill the page dir and page table */
void PageTable :: init_paging(FramePool * _kernel_mem_pool, 
			      FramePool * _process_mem_pool,
		              const unsigned long _shared_size)
{
	kernel_mem_pool = _kernel_mem_pool;
	process_mem_pool = process_mem_pool;
	shared_size = _shared_size;
	PageTable :: address = 0; // holds the phy address of where the page is
	// map the first 4MB of memory; direct mapping
	for (int i = 0; i < 1024; i++)
	{
		page_table[i] = address | 3; // supervisor, r/w, present
		FramePool :: _frame_bitmap[i] = 1; // set the bitmap, reserved
		address += PG_SIZE; // addr are page aligned
	}

	/* Fill in the page dir entries */	
	// The first entry of the page dir
	page_directory[0] = page_table; // point to the first page table
	page_directory[0] = page_directory[0] | 3; // supervisor, r/w, present

	/* set up the attribute for the remaining entries in the page dir */
	for (i = 1; i < 1024; i++)
		page_directory[i] = 0 | 2; // supervisor, r/w, not present
}

/* load into processor context */
void PageTable :: load()
{
	current_page_table = this; // load the current page table
}

/* set up paging */
void PageTable :: enable_paging()
{
	/* add address of page dir to cr3 and set paging bit of cr0 to 1*/
	write_cr3(page_directory);
	write_cr0(read_cr0() | 0x80000000);
}

/* page fault handler */
void PageTable :: handle_fault(REGS * _r)
{
	/* check error code */
	int present_bit = _r->err_code & 0x1; // present bit not set?
	int rw_bit = _r->err_code & 0x2; // write operation?
	int user_bit = _r->err_code & 0x4; // process in user mode?
	int reserved_bit = _r->err_code & 0x8; // overwritten reserved bits of PT?
	/* present bit not set in page? */
	if (present_bit != 1)
		Console::puts("Page fault because page not present \n");
	/* read faulting address from cr2 */
	unsigned long fault_addr = read_cr2();
	/* compute the page table index */
	unsigned long page_table_index = (fault_addr >> 12) & MASK;
	/* compute the page dir index */
	unsigned long page_dir_index = (fault_addr >> 22) & MASK;
	/* check if page directory is valid*/
	if (page_directory[page_dir_index] == 2)
	{
		/* create page table and assign frame */
		page_table = kernel_mem_pool->get_frame();
		unsigned long frame_addr = process_mem_pool->get_frame();
		/* assign frame addr */
		page_table[page_table_index] = frame_addr | 3;
		/* point the page table to the page dir */
		page_directory[page_dir_index] = page_table;
		/* update the dir */
		page_directory[page_dir_index] |= 3; // set to supervisor,r/w,present
	}
	else
		/* page table already created, just assign frame  */
	{
		/* allocate address for the frame */
		unsigned long frame_addr = process_mem_pool->get_frame();
		/* assign to the page table */
		page_table[page_table_index] = frame_addr | 3;

	}
}
