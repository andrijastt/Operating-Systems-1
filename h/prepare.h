#ifndef PREPARE_H_
#define PREPARE_H_

#include "ivtentry.h"
#include "kernelev.h"

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IvtEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
	if (newEntry##numEntry.owner) newEntry##numEntry.owner->signal();\
	if (callOld == 1)\
		newEntry##numEntry.callOldRoutine();\
	dispatch();\
}

#endif /* PREPARE_H_ */
