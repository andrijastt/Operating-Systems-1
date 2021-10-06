#include "list.h"
#include "SCHEDULE.H"

List::List(){
	head = 0;
	tail = 0;
}

void List::addAtEnd(PCB* pcb){

	Elem* newly = new Elem(pcb);

	if(head == 0) head = tail = newly;
	else tail = tail->next = newly;

}

void List::removeElement(PCB* pcb){

	if(head == 0) return;

	if(head->pcb == pcb){
		Elem* temp = head;
		if(head == tail) tail = 0;
		head = head->next;
		temp->pcb = 0;
		delete temp;
		return;
	}

	for(Elem* e = head; e->next; e = e->next){

		if(e->next->pcb == pcb){
			Elem* temp = e->next;
			if(e->next == tail) tail = e;
			e->next = temp->next;
			temp->next = 0;
			temp->pcb = 0;
			delete temp;
			return;
		}
	}

}

Thread* List::getThreadById(int id){

	if (head == 0) return 0;

	for(Elem* e = head; e; e=e->next){
		if(e->pcb->id == id) return e->pcb->owner;
	}

	return 0;
}

List::~List(){

	while (head){
		Elem *temp = head;
		head = head -> next;
		temp->pcb = 0;
		delete temp;
	}

	tail = 0;
}

void List::waitListEmpty(){

	while (head){
		Elem* temp = head;
		head = head->next;
		temp->pcb->status = READY;
		Scheduler::put(temp->pcb);
		temp->pcb = 0;
		delete temp;
	}

	tail = 0;
}

PCB* List::putToSchedulerFirst(){

	if(head == 0) return 0;

	Elem* temp = head;
	head = head->next;
	if(head == 0) tail = 0;

	temp->pcb->status = READY;
	temp->pcb->wait = -1;				// Not waiting
	Scheduler::put(temp->pcb);
	temp->next = 0;
	return temp->pcb;
}

void List::reduceByOne(){

	if(head == 0) return;

	Elem* temp = head;
	while(temp){

		Elem* next = temp->next;
		temp->pcb->wait--;
		if(temp->pcb->wait == 0){

			temp->pcb->status = READY;
			Scheduler::put(temp->pcb);

			temp->pcb->semaphore->inc();
			temp->pcb->semaphore = 0;
			temp->pcb->semaphore->blocked->removeElement(temp->pcb);
		}

		temp = next;
	}

	temp = head;
	while(temp){
		Elem* next = temp->next;
		if(temp->pcb->wait == 0) KernelSem::blockedWait->removeElement(temp->pcb);
		temp = next;
	}
}
