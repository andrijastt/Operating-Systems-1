#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "event.h"
#include "pcb.h"
#include "ivtentry.h"

class KernelEv{

	friend class Event;

	int value;
	PCB* owner;
	IVTNo ivtNumber;

	KernelEv (IVTNo ivtNo);
	~KernelEv ();

	void wait ();
public:
	void signal();
};


#endif /* KERNELEV_H_ */
