#include "kernelev.h"
#include "SCHEDULE.H"
#include "ivtentry.h"

KernelEv::KernelEv(IVTNo ivtNo){
	owner = (PCB*) PCB::running;
	value = 0;
	ivtNumber = ivtNo;
	IvtEntry::getIvtEntry(ivtNumber)->owner = this;
}

void KernelEv::signal(){

	System::lock();

	if(value == 0 && owner->status == BLOCKED){
		owner->status = READY;
		Scheduler::put(owner);
	}
	else value = 1;

	System::unlock();
}

void KernelEv::wait(){

	if(owner != PCB::running) return;

	if(value == 0){
		PCB::running->status = BLOCKED;
		dispatch();
	}
	else value = 0;
}

KernelEv::~KernelEv(){
	if(IvtEntry::getIvtEntry(ivtNumber)) IvtEntry::getIvtEntry(ivtNumber)->owner = 0;
}
