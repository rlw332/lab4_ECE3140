#include <stdlib.h>
#include "3140_concur.h"
#include "lock.h"

void l_init(lock_t* l) {
	__disable_irq(); //this part is optional but does not change much
	l -> flag = 0; // initialize lock struct members
	__enable_irq();
}

void l_lock(lock_t* l) {
	while(1) {
		__disable_irq(); //disable interrupts to ensure atomicity
		if(l -> flag == 0) { // if lock is not acquired then acquire it and return
			l -> flag = 1;
			__enable_irq();
			return;
		} // if lock is acquired enable interrupts for a second and keep spinning
		__enable_irq();
	}
}

void l_unlock(lock_t* l) {
	__disable_irq(); // optional but good to ensure fairness
	l -> flag = 0; // free the lock
	__enable_irq();
}
