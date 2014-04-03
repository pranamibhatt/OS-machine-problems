/**	
Name: frame_pool.C
Author: Pranami Bhattacharya	
Frame pool manager
 **/

#include "frame_pool.H"
#include "console.H"
#include "utils.H"

#define KERNEL_START_FRAME	512
#define PROCESS_START_FRAME	1024
#define KERNEL_BASE_ADDR	0x200000 // 2MB
#define PROCESS_BASE_ADDR	0x400000 // 4 MB
#define PG_SIZE			4096

FramePool::FramePool(unsigned long _base_frame_no, unsigned long _nframes, unsigned long _info_frame_no)
{
	base_frame_no = _base_frame_no;
	nframes = _nframes;
	info_frame_no = _info_frame_no;

	// the base address assigned for the kernel mem pool and process mem pool
	if(_base_frame_no == KERNEL_START_FRAME) //kernel mem pool
	{
		base_address = 0x200000;
		frame_bitmap = (unsigned long *)KERNEL_BASE_ADDR;

		// reserve frame 0 to hold the bitmap info and mark as taken
		frame_bitmap[0] = 1;
		for (int i = 1; i < nframes; i++)
			frame_bitmap[i] = 0; 
	}
	else if(_base_frame_no == PROCESS_START_FRAME) //process mem pool
	{
		base_address = 0x400000; 
		// compute the process mem pool addr given the info_frame_no
		frame_bitmap = (unsigned long *)(KERNEL_BASE_ADDR + ((_info_frame_no-512) * PG_SIZE));
	//	frame_bitmap = (unsigned long *)base_address;
		// reserve first frame to hold the bitmap
		for (int i = 1; i < nframes; i++)
			frame_bitmap[i] = 0; 
	}    
}

unsigned long FramePool::get_frame()
{
	int count = 0;
	int hit_flag = 0;
	for(; count < nframes; count++)
	{
		// the first posn with bit unset
		if (frame_bitmap[count] == 0)
		{
			hit_flag = 1;
			break; 
		}
	}

	// free posn exists inside the frame pool
	if(hit_flag == 0) 
		return 0;

	// mark the posn as taken
	frame_bitmap[count] = 1;
	return (base_frame_no + count);
}

#if 1
unsigned long FramePool::get_frame_address(unsigned long _frame_no)
{
	return base_address + ((_frame_no-base_frame_no) * PG_SIZE);
}

#endif
void FramePool::mark_inaccessible(unsigned long _start_frame, unsigned long _nframes)
{
	// mark the region as inaccessible

	int count = 0;
	for (count = 0; count < (_start_frame+_nframes); count++)
	{
		frame_bitmap[count] = -1;
	}
}

void FramePool::release_frame(unsigned long _frame_no)
{
	frame_bitmap[_frame_no] = 0;
}
