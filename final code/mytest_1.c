//TEST CASE 1
//This test case has to do with what happens when two processes both require two locks (i.e. both processes are doubled locked)
//For spin lock, we expect the program to first go into p1 
//For blocking lock, we expect...

#include "3140_concur.h"
#include "led.h"
#include "lock.h"

lock_t l1;
lock_t l2;

void p1(void) {
    l_lock(&l1);
    delay(100);
    l_lock(&l2);  //Acquire the second lock


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
    l_unlock(&l2);
}

void p2(void) {
    l_lock(&l1);
    delay(100);
    l_lock(&l2);  //Acquire the first lock


    //Critical Section - should toggle green on and red on/off twice
    green_toggle_frdm();
    delay(300);
    red_toggle_frdm();
    delay(300);
    red_toggle_frdm();
    delay(300);
    red_toggle_frdm();
    delay(300);
    red_toggle_frdm();

    l_unlock(&l1);
    l_unlock(&l2);
}

int main(void) {
    led_init();
    l_init(&l1);
    l_init(&l2);

    if (process_create(p1, 20) < 0) {
        return -1;
    }
    if (process_create(p2, 20) < 0) {
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

    red_on();
    blue_on();

    while (1);
}
