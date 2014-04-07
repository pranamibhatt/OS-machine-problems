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
	Thread *thr = q.dequeue ();	
	Thread :: dispatch_to (thr);	
}

// put thread back to the end of the queue
void Scheduler :: resume (Thread * _thread)
{
	// enqueue thread to the end of the Q
	q.enqueue (_thread);
}

void Scheduler :: add (Thread * _thread)
{
	// make thread runnable and add it to the Q
	q.enqueue (_thread);
}

void Scheduler :: terminate (Thread * _thread)
{
	// remove _thread for the queue and destroy
	q.remove (_thread);
}

void Rqueue :: enqueue (Thread * thrd)
{
	//pushing the thread to the end of the queue
	// queue empty?
	if (head == tail)
		
		queue[head] = thrd;
	else
		queue[tail] = thrd;
	tail++;
}

// return the thread at the head of the q
Thread *Rqueue :: dequeue ()
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
	thr = queue[head];
	for (i = head; i < tail; i++)
		queue[i-1] = queue[i];
	// make the last place available
	queue[tail-1] = 0;
	tail--;
	return thr; 
	}
	
}

#if 1
// remove the given thread from the scheduler, needed for thread termination
void Rqueue :: remove (Thread *thrd)
{
	int i;
	// find the thread with the thread id	
	for (i = head; i < tail; i++)
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
	for (int j = i+1; j < tail; j++)
		queue[j-1] = queue[j];
	// make last place available
	queue[tail-1] = 0;
	tail--;
}
#endif
