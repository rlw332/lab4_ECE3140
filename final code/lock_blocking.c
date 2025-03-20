#include <stdlib.h>
#include "3140_concur.h"
#include "lock.h"

void l_init(lock_t* l) {
	__disable_irq();
	l -> flag = 0;
	l -> process_queue = malloc(sizeof(process_queue_t));
	l -> process_queue -> head = NULL;
	__enable_irq();
}

void l_lock(lock_t* l) {
	while(1) {
		__disable_irq();
		if(l -> flag == 0) {
			l -> flag = 1;
			__enable_irq();
			return;
		} else {
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
	__disable_irq();
	l -> flag = 0;
	//current_process_p -> blocked = 1; // i think this line is optional (??)
	//enqueue(current_process_p, l -> process_queue);
	process_t *blocked_process = dequeue(l -> process_queue);
	if(blocked_process != NULL){
		blocked_process -> blocked = 0;
		enqueue(blocked_process, &process_queue);
	}
	__enable_irq();
}
