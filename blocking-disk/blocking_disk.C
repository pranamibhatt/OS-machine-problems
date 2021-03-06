/**
	Name: blocking_disk.C
	Author: Pranami Bhattacharya
	Desc: blocking disk functions
**/

#include "blocking_disk.H"
#include "thread.H"
#include "scheduler.H"
#include "utils.H"
#include "console.H"

// alternate implementation technique could be to keep the read and write function 
// in simple disk and override the wait_until_ready function
// redefine wait_until_ready here in blocking disk

extern Scheduler *SYSTEM_SCHEDULER;


void BlockingDisk :: read (unsigned long _block_no, char *_buf)
{
   /* Reads 512 Bytes in the given block of the given disk drive and copies them 
   to the given buffer. No error check! */

  issue_operation(READ, _block_no);

  //wait_until_ready();
   // if the block is not ready to be read from
   while (!SimpleDisk::is_ready())
   {
	bq->add(Thread::CurrentThread());
	SYSTEM_SCHEDULER->yield();	
   }
	
  /* read data from port */
  int i;
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = inportw(0x1F0);
    _buf[i*2]   = (unsigned char)tmpw;
    _buf[i*2+1] = (unsigned char)(tmpw >> 8);
  }	
}

void BlockingDisk :: write (unsigned long _block_no, char *_buf)
{
/* Writes 512 Bytes from the buffer to the given block on the given disk drive. */

  issue_operation(WRITE, _block_no);

  while (!SimpleDisk::is_ready())
  {
	bq->add(Thread::CurrentThread());
	SYSTEM_SCHEDULER->yield();
  }

  /* write data to port */
  int i; 
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = _buf[2*i] | (_buf[2*i+1] << 8);
    outportw(0x1F0, tmpw);
  }
	
}

// add to the blocking queue
void Bqueue :: add (Thread * _thread)
{
	// queue empty?
	if (head == tail)
		b_queue[head] = _thread;
	else
		b_queue[tail] = _thread;
	tail++;
}

// remove from the blocking queue
Thread * Bqueue :: del ()
{
	Thread *thr;
	int i;
	// remove thread from the beginning of the queue
	if (head == tail)
	// nothing to dequeue
		return NULL;
	else
	{
	// return the thread at the head
	thr = b_queue[head];
	for (i = head; i < tail; i++)
		b_queue[i-1] = b_queue[i];
	// make the last place available
	b_queue[tail-1] = 0;
	tail--;
	return thr; 
	}
}

// get first thread from the blocking device Q
// remove from the blocking queue
Thread * BlockingDisk :: get_disk_thread ()
{
	// is the disk ready?
	if (SimpleDisk::is_ready())
	{
		return bq->del();
	}
	else
	{
		return NULL;
	}
}
