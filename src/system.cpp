#include <dos.h>

#include "system.h"
#include "thread.h"
#include "SCHEDULE.h"
#include "kernelsm.h"

static unsigned tsp;
static unsigned tss;
static unsigned tbp;

IdleThread* System::idle = 0;
PCB* System::mainThread = 0;

pInterrupt System::oldTimer = 0;
volatile Time System::counter = 0;

volatile int System::explicit_change_of_context = 0;
int System::lockFlag = 0;

List* KernelSem::blockedWait = new List();

extern void tick();

void System::init(){
	asm cli;

#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(0x08);
	setvect(0x08, timer);
	setvect(0x60, oldTimer);
#endif

	idle = new IdleThread();

	PCB::running = (volatile PCB*) new PCB(0, 0, defaultTimeSlice);
	mainThread = (PCB*)PCB::running;
	PCB::running->status = RUN;

	asm sti;
}

void System::restore(){
	asm cli

#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldTimer);
#endif

	delete KernelSem::blockedWait;

	mainThread->status = FINISHED;
	delete mainThread;

	idle->myPCB->status = FINISHED;
	delete idle;

	asm sti;
}

void System::dispatch(){
	asm cli;
	explicit_change_of_context = 1;
	System::timer();
	asm sti;
}

void interrupt System::timer(...){

	if (!explicit_change_of_context) {

		tick();

		KernelSem::blockedWait->reduceByOne();

		if(PCB::running->quantum != 0) counter--;

		asm int 60h;
	}

	if(explicit_change_of_context || (counter == 0 && PCB::running->quantum != 0)){
		if(lockFlag == 0){
			explicit_change_of_context = 0;
			asm{
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if(PCB::running != (volatile PCB*)idle && PCB::running->status == RUN){
				PCB::running->status = READY;
				Scheduler::put((PCB*)PCB::running);
			}

			PCB::running = Scheduler::get();
			if(PCB::running == 0) PCB::running = idle->myPCB;
			PCB::running->status = RUN;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tsp = PCB::running->bp;

			counter = PCB::running->quantum;

			asm{
				mov ss, tss
				mov sp, tsp
				mov bp, tbp
			}
		}
		else {
			explicit_change_of_context = 1;
		}
	}
}

void System::lock(){
	System::lockFlag++;
}

void System::unlock(){
	if((--System::lockFlag == 0) && explicit_change_of_context) dispatch();
}
