#include <stdio.h>
#include <stdlib.h> //exit
#include <sys/mman.h> //mlockall
#include <native/task.h>
#include <native/timer.h>
#include <wiringPi.h>


#define TASK_PRIO 99 // 99 is Highest RT priority, 0 is Lowest
#define TASK_MODE 0 // No flags
#define TASK_STKSZ 0 // default Stack size 
#define TASK_PERIOD 200000 // 0.5= 50000000 ns

RT_TASK tA;

void periodic_task (void *arg) {

RTIME now, previous;

previous= rt_timer_read();

for (;;) {
 //task migrates to primary mode with xeno API call

 rt_task_wait_period(NULL); //deschedule until next period. 
digitalWrite(0,HIGH);
 now = rt_timer_read(); //cureent time

 //task migrates to secondary mode with syscall
 //so printf may have unexpected impact on the timing 

digitalWrite(0,LOW);
 printf("Time elapsed: %ld.%04ld ms\n",
 (long)(now - previous) / 1000000,
 (long)(now - previous) % 1000000);
 previous = now;

}

}

int main (int argc, char *argv[]) {

wiringPiSetup();
pinMode(0,OUTPUT);

int e1, e2, e3, e4;

RT_TIMER_INFO info;
mlockall(MCL_CURRENT|MCL_FUTURE);
e1 = rt_timer_set_mode(TM_ONESHOT); // Set oneshot timer 
e2 = rt_task_create(&tA, "periodicTask", TASK_STKSZ, TASK_PRIO, TASK_MODE);

//set period
e3 = rt_task_set_periodic(&tA, TM_NOW, rt_timer_ns2ticks(TASK_PERIOD));
e4 = rt_task_start(&tA, &periodic_task, NULL); 

if (e1 | e2 | e3 | e4) {
 fprintf(stderr, "Error launching periodic task....\n");
rt_task_delete(&tA);
exit(1);

}

printf("Press any key to end....\n");
 getchar();
rt_task_delete(&tA);
return 0;
}

