#include "idle.h"

IdleThread::IdleThread():Thread(defaultStackSize, 1){}

void IdleThread::run(){

	for(int i = 0; i < 30; i++){

		for(int j = 0; j < 10000; j++)
			for(int k = 0; k < 30000; k++);
		if (i == 29) i = 0;
	}


}
