#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "idle.h"
#include "pcb.h"
#include "kernelsm.h"
#include "semaphor.h"
#include "list.h"
#include "event.h"
#include "kernelev.h"
#include "ivtentry.h"

typedef void interrupt (*pInterrupt)(...);

class System{

public:

	static void init();
	static void restore();

	static void dispatch();

	static void lock();
	static void unlock();

private:

	friend class PCB;
	friend class KernelSem;
	friend class ListSem;
	friend class Event;
	friend class KernelEv;

	static void interrupt timer(...);

	static IdleThread* idle;
	static PCB* mainThread;

	static pInterrupt oldTimer;
	static volatile Time counter;
	static volatile int explicit_change_of_context;

	static int lockFlag;

};

#endif /* SYSTEM_H_ */
