//TEST CASE 1
//Comments about behavior at the bottom

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
    delay(150);
    l_unlock(&l1);
    
    red_toggle_frdm();
    delay(100);
    red_toggle_frdm();
    delay(100);
    
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

//This test case has to do with what happens when two processes both require 
//two locks (i.e. both processes are doubled locked)

//For spin lock, we expect the program to first create two locks l1 and l2. Then the process p1 will attempt to,
//acquire the locks by spinning (checking continuously if they are available). Since the locks are available, the program goes into p1
//where the process will lock l1 and then lock l2 after a short delay. Then p1 will enter its critical section 
//and completes the process of turning on and off the red FRDM led on for a medium, then long, then short amount of time
//After p1 completes its critical section, it unlocks the locks l1 and l2 and jumps into the p2 program (which has failed to acquire the locks initially 
//and has been spinning (checking continuously until the locks are available), unable to proceed, until now where it 
//completes the process of toggling on the green FRDM led, and turning on and off the red FRDM twice before finishing
//its critical section and moving on to the final process in the init where it toggles on the FRDM 
//the green off and then the red on, and then the green on and then the red off before turning the blue and red LEDS in LED0 on

//For blocking lock, we expect the same program to happen (a little faster), but the process in which the locks are used will vary
//In this case after the locks l1 and l2 are initializes, p1 will try to lock l1 if it is available (which it is) and then moments later lock l2
//The process p2 will also try to acquire the locks l1 and l2, but since p1 already has them, p2 will block the locks at the l_lock(&l1)  
//and l_lock(&l2) function calls, and wait until p1 releases the locks by first completeing its critcial section and then unlocking the locks for use
//which p2 can now lock, using them to enter the critical section before unlocking and continuing with the processes in the init
