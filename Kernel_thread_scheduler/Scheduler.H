/**
	Name: Scheduler.H
	Author: Pranami Bhattacharya
	Desc: the scheduler header

**/

#include "thread.H"
#include "machine.H"
#include "utils.H" // memset

#define MAX_THREADS 10
/* class declaration*/

// ready queue
class Rqueue
{
	// an array of pointers to an array of threads
	Thread **queue;
	// index to the front and back of the queue
	int head, tail;
	public :
	// insert into the q
	void enqueue (Thread *);
	// delete from the q
	Thread * dequeue ();
	// for terminate
	void remove (Thread *);
	Rqueue()
	{
		queue = new Thread *[MAX_THREADS];
		head = tail = 0;
		// making the queue 0 initially
		memset (queue, 0, sizeof(Thread *));
	}
		
	
};
// scheduler class
class Scheduler
{
	// object to the queue
	Rqueue q;
	// for round robin scheduling
	public:
	static int handle_flag;
	//static int RRschedule;
	// set up the scheduler
	Scheduler();

	// currently running thread gives up CPU
	virtual void yield ();

	// add the given thread to the ready queue 
	virtual void resume (Thread * _thread);

	//make the given thread runnable
	virtual void add (Thread * _thread);

	//remove the thread from the scheduler
	virtual void terminate (Thread * _thread);

	//handle round robin scheduling
	virtual void handle_RRinterrupt (REGS *_r);
};
