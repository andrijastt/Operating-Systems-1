#include <dos.h>

#include "pcb.h"
#include "SCHEDULE.H"
#include "system.h"

List* PCB::threads = new List();	// Initalization of static attributes
ID PCB::globalID = -1;				// -1 is for Idle thread, 0 is for mainThread, 1+ are for user threads
volatile PCB* PCB::running = 0;

void PCB::start(){

	if(status == READY || status == FINISHED) return;
	status = READY;					// Set status to ready and put it in scheduler
	Scheduler::put(this);

}

ID PCB::getId(){

	return id;

}

ID PCB::getRunningId(){

	return ((PCB*)running) -> id;
}

PCB::PCB(Thread* thread,  StackSize stackSize, Time timeSlice){
	owner = thread;
	waiting = new List();

	quantum = timeSlice;
	id = globalID++;

	status = NEW;

	if(stackSize > 65535) stackSize = 65535;
	stackSize /= sizeof(unsigned);

	stack = new unsigned[stackSize];
	if (stackSize > 13) {
		stack[stackSize - 1] = 0x200;

#ifndef BCC_BLOCK_IGNORE
		stack[stackSize - 2] = FP_SEG(wrapper);
		stack[stackSize - 3] = FP_OFF(wrapper);

		sp = FP_OFF(stack + stackSize - 12);
		ss = FP_SEG(stack + stackSize - 12);
		bp = sp;
#endif
	}

	semaphore = 0;

	wait = -1;	// Initialy

	threads->addAtEnd(this);
}

Thread* PCB::getThreadById(int id){
	return threads->getThreadById(id);

}

void PCB::waitToComplete(){

	if(status == NEW || status == FINISHED || this == running || this == System::idle) return;
	// Cases where you cant call waitToComplete

	System::lock();
	//running->waiting->addAtEnd(this);
	waiting->addAtEnd((PCB*)PCB::running);
	running->status = BLOCKED;
	System::unlock();

	dispatch();
}

PCB::~PCB(){
	delete stack;
	delete waiting;
}

void PCB::wrapper(){

	PCB::running->owner->run();

	System::lock();
	PCB::running->status = FINISHED;
	PCB::running->waiting->waitListEmpty();
	System::unlock();

	dispatch();
};
