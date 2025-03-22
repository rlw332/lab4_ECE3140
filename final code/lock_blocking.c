#include <stdlib.h>
#include "3140_concur.h"
#include "lock.h"

void l_init(lock_t* l) {
	__disable_irq(); //optional
	l -> flag = 0; // initialize flag
	l -> process_queue = malloc(sizeof(process_queue_t)); // allocate memory for the queue in regards to the lock
	l -> process_queue -> head = NULL; // set queue as empty for now
	__enable_irq();
}

void l_lock(lock_t* l) {
	while(1) { //this loop differs from spin lock as it almost never gets executed unless absolutely necessary (e.g. freed lock or first check)
		__disable_irq(); // atomicity
		if(l -> flag == 0) { // if lock is free then acquire and we are done
			l -> flag = 1;
			__enable_irq();
			return;
		} else { // otherwise add tthis process to the queue for this lock if not already added 
			// (could already be added if another process 'snipes' the lock immediately after freed if it perfectly asks for lock then)
			if(current_process_p -> blocked == 0) {
				enqueue(current_process_p, l -> process_queue);
				current_process_p -> blocked = 1;
			}
			__enable_irq();
			process_blocked();
		}
	}
}

void l_unlock(lock_t* l) {
	__disable_irq(); // optional but good for fairness
	l -> flag = 0; // free lock
	process_t *blocked_process = dequeue(l -> process_queue); // get one of blocked processes and unblock it (wake it up)
	if(blocked_process != NULL){ // need to check if queue was empty because then calling its member could be undefined behavior
		blocked_process -> blocked = 0;
		enqueue(blocked_process, &process_queue);
	}
	__enable_irq();
}
