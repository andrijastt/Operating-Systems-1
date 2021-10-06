#include <dos.h>
#include "ivtentry.h"
#include "system.h"

IvtEntry* IvtEntry::ivtTable[256] = {0};

IvtEntry* IvtEntry::getIvtEntry(IVTNo ivtNo){ return ivtTable[ivtNo]; }

IvtEntry::IvtEntry(IVTNo ivtNo, pInterrupt p){

	System::lock();
	ivtNumber = ivtNo;
	newRoutine = p;
	owner = 0;

#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
#endif

	ivtTable[ivtNo] = this;
	System::unlock();
}

void IvtEntry::callOldRoutine(){

	if(oldRoutine) (*oldRoutine)();
}

IvtEntry::~IvtEntry(){

	System::lock();

#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNumber,oldRoutine);
#endif

	newRoutine = 0;
	oldRoutine = 0;

	ivtTable[ivtNumber]->owner = 0;
	System::unlock();
}
