
#include <stdlib.h>
#include "3140_concur.h"
#include "lock.h"

void l_init(lock_t* l) {
	__disable_irq();
	l -> flag = 0;
	__enable_irq();
}

void l_lock(lock_t* l) {
	while(1) {
		__disable_irq();
		if(l -> flag == 0) {
			l -> flag = 1;
			__enable_irq();
			return;
		}
		__enable_irq();
		delay(1);
	}
}

void l_unlock(lock_t* l) {
	__disable_irq();
	l -> flag = 0;
	__enable_irq();
}
