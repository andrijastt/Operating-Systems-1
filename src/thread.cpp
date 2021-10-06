#include "pcb.h"
#include "SCHEDULE.H"
#include "system.h"

void Thread::start(){
	System::lock();
	myPCB->start();
	System::unlock();
}

void Thread::waitToComplete(){
	myPCB->waitToComplete();
}

Thread::~Thread(){
	waitToComplete();
	delete myPCB;
}

ID Thread::getId(){
	return myPCB->getId();
}

Thread* Thread::getThreadById(int id){
	return PCB::getThreadById(id);
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	System::lock();
	myPCB = new PCB(this, stackSize, timeSlice);
	System::unlock();
}


void dispatch(){

	System::dispatch();
}
