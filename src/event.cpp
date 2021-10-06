#include "event.h"
#include "system.h"

Event::Event(IVTNo ivtNo){
	System::lock();
	myImpl = new KernelEv(ivtNo);
	System::unlock();
}

Event::~Event(){
	System::lock();
	delete myImpl;
	System::unlock();
}

void Event::signal(){

	myImpl->signal();

}

void Event::wait(){

	myImpl->wait();

}
