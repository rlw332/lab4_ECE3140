#include <stdlib.h>
#include "3140_concur.h"
#include "lock.h"

void l_init(lock_t* l) {
	__disable_irq();
	l -> flag = 0;
	l -> process_queue -> head = NULL;
	__enable_irq();
}

void l_lock(lock_t* l) {
	//while(1) {
		__disable_irq();
		if(l -> flag == 0) {
			l -> flag = 1;
			return;
		} else {
			enqueue(current_process_p, l -> process_queue);
			current_process_p -> blocked = 1;
			__enable_irq();
			process_blocked();
		}
	//}
}

void l_unlock(lock_t* l) {
	__disable_irq();
	l -> flag = 0;
	process_t blocked_process = dequeue(l -> process_queue);
	blocked_process -> blocked = 0;
	__enable_irq();
}
