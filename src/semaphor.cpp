#include "semaphor.h"
#include "kernelsm.h"

Semaphore::Semaphore(int init){
	System::lock();
	myImpl = new KernelSem(init);
	System::unlock();
}

Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait){

	System::lock();
	int ret = myImpl->wait(maxTimeToWait);
	System::unlock();

	return ret;
}

void Semaphore::signal(){
	System::lock();
	myImpl->signal();
	System::unlock();
}

int Semaphore::val() const{

	return myImpl->val();
}
