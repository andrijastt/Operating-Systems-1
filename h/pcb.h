#ifndef PCB_H_
#define PCB_H_

#include "list.h"
#include "kernelsm.h"
#include "kernelev.h"

enum Status{NEW = 0, READY, RUN, BLOCKED, FINISHED};	// Status of Thread

class PCB{
private:

	friend class Thread;
	friend class List;
	friend class System;
	friend class KernelSem;
	friend class KernelEv;

	// Methods
	void start();
	void waitToComplete();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	PCB(Thread* thread, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	~PCB();

	static void wrapper();

	// Attributes

	unsigned ss,sp,bp;		// Stack segment, Stack pointer, Base pointer
	unsigned* stack;		// Stack

	Thread* owner;			// Witch thread this PCB belongs to

	ID id;					// The Id of thread
	static ID globalID;		// It's use is for threads to have different IDs

	Status status;			// Status of thread
	int quantum;			// How many timeslices thread has

	static List* threads;						// A list of threads that are NOT new PCB
	List* waiting;								// Threads on witch current thread is waiting
	static volatile PCB* running;				// Thread that is currently running

	volatile Time wait;				// How many timeslices it waits before semaphore lets it through

	KernelSem* semaphore;
};



#endif /* PCB_H_ */
