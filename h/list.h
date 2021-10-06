#ifndef LIST_H_
#define LIST_H_

#include "thread.h"
#include "pcb.h"
#include "semaphor.h"
#include "system.h"
#include "kernelsm.h"

class List{

	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB* p):pcb(p), next(0){}
	};

	Elem *head, *tail;

	friend class PCB;
	friend class KernelSem;
	friend class System;

protected:

	List();
	~List();

	void addAtEnd(PCB* pcb);
	void removeElement(PCB* pcb);

	void waitListEmpty();
	Thread* getThreadById(ID id);

	void reduceByOne();
	PCB* putToSchedulerFirst();

};

#endif /* LIST_H_ */
