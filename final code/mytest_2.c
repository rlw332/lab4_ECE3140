//TEST CASE 2
//This test case has to do with what happens when two processes both require two locks
//For spin lock, we expect...
//For blocking lock, we expect...

#include "3140_concur.h"
#include "led.h"
#include "lock.h"

lock_t l1;

void p1(void) {
    l_lock(&l1);
    delay(100);

    //Critical Section - should go red medium, red long, red short before unlocking
    red_toggle_frdm();
    delay(300);
    red_toggle_frdm();
    delay(300);

    red_toggle_frdm();
    delay(900);
    red_toggle_frdm();
    delay(300);

    red_toggle_frdm();
    delay(150);
    red_toggle_frdm();

    l_unlock(&l1);
}


int main(void) {
    led_init();
    l_init(&l1);

    if (process_create(p1, 20) < 0) {
        return -1;
    }

    //
    process_start();
    delay(300);
    green_toggle_frdm();
    delay(300);
    red_toggle_frdm();
    delay(300);
    green_toggle_frdm();
    delay(300);
    red_toggle_frdm();
    delay(300);

    blue_on();
    green_on();

    while (1);
}
