/**
**	Name: frame_pool.C
**	Author: Pranami Bhattacharya
**	Desc: Frame Management
**	Date: 02-15-14
**/

/**
	Frame Management Steps:
	Kernel data between 2MB-4MB
	Non kernel data above 4 MB
**/

#include "frame_pool.H"
#define PG_SIZE 4096
unsigned long FramePool :: base_frame_no;
unsigned long FramePool :: nframes;
unsigned long FramePool :: info_frame_no;
unsigned long FramePool :: _frame_bitmap[8192] {}; // setting up bitmap of 8192 bytes should suffice for 
						// the current mem requirements
/* FramePool constructr */
FramePool :: FramePool(unsigned long _base_frame_no, unsigned long _nframes, unsigned long _info_frame_no)
{
	base_frame_no = _base_frame_no;
	nframes = _nframes;
	info_frame_no = _info_frame_no;
	
}

// allocate a frame from the pool
// return frame number on success, 0 on failure
unsigned long  FramePool :: get_frame()
{
	for (unsigned long i = 0; i < nframes; ++i) 
	{
		/* find first frame with bitmap not set */
		if ((_frame_bitmap[i]) == 0)
		{
			/* set the bitmap to 1 and return the frame */
			_frame_bitmap[i] == 1;
				return ((i*PG_SIZE) + base_frame_no);
		}
	}
}

// mark this area of phy memory as inaccessible
void FramePool :: mark_inaccessible(unsigned long _base_frame_no, unsigned long _nframes)
{	
	for (unsigned long i = _base_frame_no; i < (base_frame_no + _nframes); ++i)
		FramePool :: _frame_bitmap[i] = -1;
}

// release the frame back to the frame pool
static void FramePool :: release_frame(unsigned long _frame_no)
{
	// reset the bit
	FramePool :: _frame_bitmap[_frame_no] = 0;
}
