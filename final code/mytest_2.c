//TEST CASE 2
//Comments for instructions are at the bottom

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

//This test case has to do with what happens when one processes acquires one lock 
//In this test case we are trying to ensure everything ends up being freed properly after the locking process

//For spin lock, we expect the program to first create a lock l1, where p1 will try to acquire it (if it is unable to, it will 
//be unable to move on until the lock is available). Since there are no other programs here, p1 is easily able to acquire the lock l1
//and the program will continue with process p1. Then p1 will enter its critical section where it
//completes the process of turning on and off the red FRDM led on for a medium, then long, then short amount of time (same as before).
//After p1 completes its critical section, it unlocks the locks l1. Since there are no other processes spinning, waiting to acquire the lock l1
//there is no risk of deadlock, and the program goes into the init section where it turns the the green FRDM led on, then the red one, before turning the green FRDM off and then the red one,
//before ending by turning on the green and blue LEDS in LED0, making a teal color.

//For blocking lock, we expect the same program to happen. The program will first make the lock l1, where p1 will try to acquire it and be successful,
//since l1 is unlocked initially and there is no competing program (which would have blocked until the locks were avaialble)
//From here the program will act in the exact same way (with the exact same speed as the spin lock case), completing the red medium, long, short 
//sequence before unlocking the lock l1 and continuing to complete the green FRDM on, red FRDM on, green FRDM off, red FRDM off, teal (LED0) sequence.
