/**	
	Name: Scheduler.C
	Author: Pranami Bhattacharya
	Desc: A FIFO scheduler

**/


#include "thread.H"
#include "Scheduler.H"
#include "console.H"
#include "utils.H" // memmove
// Constructor
Scheduler :: Scheduler()
{

}

void Scheduler :: yield ()
{
	// give up the CPU and dispatch control to the next thread in the ready queue
	Thread *thr = q.pop ();	
	Thread :: dispatch_to (thr);	
}

// put thread back to the end of the queue
void Scheduler :: resume (Thread * _thread)
{
	// insert thread to the end of the Q
	q.insert (_thread);
}

void Scheduler :: add (Thread * _thread)
{
	// make thread runnable and add it to the Q
	q.insert (_thread);
}

void Scheduler :: terminate (Thread * _thread)
{
	// remove _thread for the queue and destroy
	q.remove (_thread);
}

void Rqueue :: insert (Thread * thrd)
{
	//pushing the thread to the end of the queue
	// queue empty?
	if (num_threads == 0)
		queue[0] = thrd;
	else
		queue[num_threads] = thrd;
	num_threads++;
}

// return the thread at the head of the q
Thread *Rqueue :: pop ()
{
	Thread *thr;
	int i;
	// remove thread from the beginning of the queue
	if (num_threads == 0)
	// nothing to pop
		return NULL;
	else
	{
	// return the thread at the head
	thr = queue[0];
	for (i = 1; i < num_threads; i++)
		queue[i-1] = queue[i];	
	// make the last place available
	queue[num_threads-1] = 0;
	num_threads--;
	return thr; 
	}
	
}

// remove the given thread from the scheduler, needed for thread termination
void Rqueue :: remove (Thread *thrd)
{
	int i;
	// find the thread with the thread id	
	for (i = 0; i < num_threads; i++)
	if ((queue[i]->Thread::ThreadId()) == thrd->Thread::ThreadId())
		// found the thread in the queue
		break;
	else	
		// thread not present in the queue
		return;
	// ith position has the matching thread
	// destroy the thread
	queue[i] = 0;
	// move the threads up one posn from i+1
	for (int j = i+1; j < num_threads; j++)
		queue[j-1] = queue[j];
	// make last place available
	queue[num_threads-1] = 0;
	num_threads--;
}

