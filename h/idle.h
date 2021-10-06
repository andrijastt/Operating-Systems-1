#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class IdleThread: public Thread{

protected:

	friend class System;
	IdleThread();
	void run();

};



#endif /* IDLE_H_ */
