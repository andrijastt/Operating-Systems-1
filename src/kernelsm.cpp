#include "kernelsm.h"
#include "pcb.h"
#include "system.h"

KernelSem::KernelSem(int init){

	blocked = new List();
	value = init;
}

KernelSem::~KernelSem(){
	delete blocked;
}

void KernelSem::signal(){

	System::lock();
	if(value++ < 0){
		if (blocked) {
			PCB* pcb = blocked->putToSchedulerFirst();
			blockedWait->removeElement(pcb);
		}
	}
	System::unlock();
}

int KernelSem::wait(Time maxTimeToWait){

	System::lock();

	if(--value >= 0){
		System::unlock();
		return 0;			// Value greater than 0, thread doesn't wait
	}
	else {

		if(maxTimeToWait > 0){
			blockedWait->addAtEnd((PCB*) PCB::running);
		}

		PCB::running->wait = maxTimeToWait;
		PCB::running->semaphore = this;
		PCB::running->status = BLOCKED;
		blocked->addAtEnd((PCB*) PCB::running);

		System::unlock();
		dispatch();

		if(PCB::running->wait == 0){
			return 1;
		}

		return 0;			// Thread is after dispatch wasn't unblocked by signal

	}

}

int KernelSem::val() const{

	return value;
}


void KernelSem::inc(){ value++;}

void KernelSem::dec(){ value--;}


