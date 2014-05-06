/**
	Name: blocking_disk.H
	Author: Pranami Bhattacharya
	Desc: Blocking disk header

**/

#include "simple_disk.H"
#include "scheduler.H"
#include "thread.H"

#define MAX_THRDS 	10

// blocking queue
class Bqueue
{
	public:
	// an array of pointers to an array of threads in the blocking Q
	Thread **b_queue; 
	int head, tail; // beginning and end of the Q

	// add thread to the blocking disk queue
	void add (Thread *);
	// remove thread from the blocking disk queue
	Thread* del ();
	// constructor
	Bqueue()
	{
		b_queue = new Thread *[MAX_THREADS];
		head = tail = 0;
		// making the queue 0 initially
		memset (b_queue, 0, sizeof(Thread *));
	}
};

class BlockingDisk : public SimpleDisk
{
	private:
		
		DISK_ID disk_id;
		unsigned int size;
	public:
		/* create a SimpleDisk device of given size connected to the 
			master or slave slot of the ATA controller*/
		
		Bqueue *bq;
		BlockingDisk (DISK_ID _disk_id, unsigned int _size) : SimpleDisk(_disk_id, _size)
		{
			disk_id = _disk_id;
			bq = new Bqueue; //assign one blocked thread queue for each disk
		}
		/* DISK OPERATIONS */
		/* read 512 bytes from the given block and copy them to the buffer */
		void read (unsigned long _block_no, char * _buf);
	
		/* write 512 bytes from the buffer to the given block */
		void write (unsigned long _block_no, char * _buf);

		/* get thread from the disk queue */
		Thread * get_disk_thread();		
};



