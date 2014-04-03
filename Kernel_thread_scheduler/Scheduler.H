/**
	Name: Scheduler.H
	Author: Pranami Bhattacharya
	Desc: the scheduler header

**/

#include "thread.H"
#include "machine.H"
#include "utils.H" // memset

/* class declaration*/

// ready queue
class Rqueue
{
	Thread **queue;
	int num_threads;		
	public :
	// insert into the q
	void insert (Thread *);
	// delete from the q
	Thread * pop ();
	// for terminate
	void remove (Thread *);
	Rqueue()
	{
		num_threads = 0;
		// making the queue 0 initially
		//memset (queue, 0, sizeof(Thread *));
	}
		
	
};
// scheduler class
class Scheduler
{
	// object to the queue
	Rqueue q;

	public:
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
	
};
