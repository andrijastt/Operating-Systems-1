#ifndef KERNELSM_H_
#define KERNELSM_H_

#include "semaphor.h"
#include "list.h"

class KernelSem{

	friend class Semaphore;
	friend class List;
	friend class System;
	friend class ListSem;

	KernelSem(int init = 1);
	~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();

	void inc();
	void dec();

	int val() const;

	int value;

	List* blocked;
	static List* blockedWait;
};


#endif /* KERNELSM_H_ */
