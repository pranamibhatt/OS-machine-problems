/**
**	Name: page_table.C
**	Author: Pranami Bhattacharya
**	The page table implementation
** 	References:
	1. http://www.rohitab.com/discuss/topic/31139-tutorial-paging-memory-mapping-with-a-recursive-page-directory/
	
**/

#include "page_table.H"
#include "paging_low.H"
#include "console.H"

/* defines */
#define KB 		*(0x1 << 10)
#define MB 		*(0x1 << 20)
#define PG_SIZE 		4096
#define MASK			0x03FF

/* Declare static members  */
FramePool *PageTable :: kernel_mem_pool;
FramePool *PageTable :: process_mem_pool;
unsigned long PageTable :: shared_size;
PageTable *PageTable :: current_page_table;
unsigned long *PageTable :: page_directory;
unsigned long *PageTable :: page_table;
/* init vm pool count to 0 */
unsigned int PageTable :: vm_pool = 0;

/* Initialize the page tables */
PageTable::PageTable()
{
	/* DM case, fixed address assigned */
	page_directory = (unsigned long *)0x200000;
	page_table = (unsigned long *)0x201000;
	unsigned long addr = 0; //point to the phy memory

	for (int i = 0; i < 1024; i++)
	{
		page_table[i] = addr | 3; 
		addr += PG_SIZE;
	}

	/* first Page dir entry points to the page table created */
	page_directory[0] = (unsigned long)page_table | 3; // set as sup, r/w, present
	/* using the recursive trick, last page dir entry points back to the PD */
	page_directory[1023] = (unsigned long)page_directory | 3; // set as sup, r/w, present
}

void PageTable::init_paging(FramePool *_kernel, FramePool *_process, const unsigned long _shared_size)
{
	kernel_mem_pool = _kernel;
	process_mem_pool = _process;
	shared_size = _shared_size;
}

void PageTable::load()
{
	/* load the current page table */
	current_page_table = this;
	/* write the dir addr to the CR3 register  */
	write_cr3((unsigned long)page_directory);
}

void PageTable::enable_paging()
{
	/* enable the paging bit in the CR0 reg */
	write_cr0(read_cr0() | 0x80000000); 
	
}

void PageTable::handle_fault(REGS* _r)
{
	/* check what exception? */
	unsigned long faulting_addr = read_cr2();

	/* compute the index of the page dir and page table */
	int page_dir_index = faulting_addr >> 22;
	int page_table_index = (faulting_addr >> 12) & MASK;

	/* Perform the recursive page dir trick */
	unsigned long* virt_page_directory = (unsigned long*)0xFFFFF000; 
	unsigned long* virt_page_table = 0;

	/* check the present bit of the dir */
	if(virt_page_directory[page_dir_index] == 2)
	{
		/* create page table, assign new frame for the faulting address */

		unsigned long frame = process_mem_pool->get_frame();
		// mark as present
		virt_page_directory[page_dir_index] = frame | 3;

		frame = process_mem_pool->get_frame();
		virt_page_table = (unsigned long*)(0xFFC00000 | (page_dir_index << 12));

		for(int i = 0; i < 1024; i++)
		//  mark as absent
			virt_page_table[i] = 0 | 2; 
		// mark as present the page containing the faulting addr
		virt_page_table[page_table_index] = frame | 3; 
	}
	else
	{
		// page table present, assign page

		unsigned long new_frame = process_mem_pool->get_frame_address(process_mem_pool->get_frame());
//		unsigned long new_frame = process_mem_pool->get_frame();
		virt_page_table = (unsigned long*)(0xFFC00000 | (page_dir_index << 12));
		// page containing the faulting address to be marked present	
		virt_page_table[page_table_index] = new_frame | 3; 
	}
}

// get the page number and release the corresponding frame
void PageTable::free_page(unsigned long _page_no)
{
	// apply mask to get the index to the page table and the page dir
	int pg_dir_idx = _page_no >> 22;
	int pg_tab_idx = (_page_no >> 12) & MASK;
	unsigned long* virt_page_table = (unsigned long*)(0xFFC00000 | (pg_dir_idx << 12));

	process_mem_pool->release_frame(virt_page_table[pg_tab_idx]);
}


/* register the VM pool
 */
void PageTable::register_vmpool(VMPool* _pool)
{
	// get a frame from the kernel pool to store the vm bitmap
	unsigned char * vm_bitmap = (unsigned char *)kernel_mem_pool->get_frame();
	VMPool** registered_pools = (VMPool**)vm_bitmap;
	/* assign the current pool to the vm pool */
	registered_pools[vm_pool] = _pool;
	vm_pool++;
}

