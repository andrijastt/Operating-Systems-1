#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "kernelev.h"

typedef void interrupt (*pInterrupt)(...);

class IvtEntry{

	friend class KernelEv;

public:
	IvtEntry(IVTNo ivtNo, pInterrupt p);
	~IvtEntry();

	pInterrupt oldRoutine, newRoutine;
	IVTNo ivtNumber;
	KernelEv* owner;

	void callOldRoutine();

	static IvtEntry* ivtTable[256];
	static IvtEntry* getIvtEntry(IVTNo ivtNo);
};

#endif /* IVTENTRY_H_ */
